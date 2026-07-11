# Project SENTINEL — CLAUDE.md
## Distributed Industrial Safety Monitor (Mini SCADA System)

---

## What This Project Is

A 3-tier distributed embedded system that mirrors the architecture used in real industrial facilities (server rooms, battery rooms, factory floors). The correct one-line description for recruiters:

> "A miniature SCADA system."

**Resume entry:**
> SENTINEL — Distributed industrial safety monitor: STM32 (FreeRTOS, 7 tasks) + Arduino (I2C sensor node) + Raspberry Pi (Python daemon, Flask dashboard). Custom binary UART protocol with CRC, fail-safe node timeout detection, hardware watchdog, MISRA-C practices, real-time actuator response <20ms.

---

## System Architecture

```
[SENSORS]──────►[ARDUINO]──I2C──►[STM32 + FreeRTOS]──UART──►[RASPBERRY PI]
                  slave             master / controller         supervisor
                                         │
                                    [ACTUATORS]
                                 relay, servo, stepper
                                 buzzer, LCD, LEDs
```

- **Tier 1 — Arduino Uno**: I2C slave, field sensor node
- **Tier 2 — STM32 Nucleo F401RE**: I2C master, real-time controller, FreeRTOS
- **Tier 3 — Raspberry Pi**: UART supervisor, web dashboard, historical logging

---

## Repository Structure

```
sentinel/
├── firmware/              ← STM32 project (CMake, not CubeIDE project files)
│   ├── Core/
│   ├── FreeRTOS/
│   ├── Drivers/
│   │   ├── rfid/
│   │   ├── lcd/
│   │   ├── keypad/
│   │   └── protocol/      ← custom UART framing library
│   └── CMakeLists.txt
├── arduino/               ← Arduino sketch (treated as firmware)
│   └── sensor_node/
├── supervisor/            ← Pi Python application
│   ├── sentinel_daemon.py
│   ├── app.py
│   ├── models.py
│   └── sentinel.service   ← systemd unit file
├── docs/
│   ├── architecture.md
│   ├── protocol_spec.md   ← document your protocol like an engineer
│   └── wiring_diagram/
└── README.md
```

---

## Tier 1 — Arduino Uno (I2C Slave / Field Sensor Node)

**Role:** Offload slow/analog sensor reading from the STM32. Dumb peripheral that exposes sensor data over I2C on demand.

### Sensors

| Sensor | Type | Measures |
|--------|------|----------|
| DHT11 | Digital | Temperature + humidity |
| LM35 | Analog | Temperature (second reading for cross-validation) |
| Photoresistance | Analog | Ambient light level |
| Big Sound Module | Digital | Noise level detection |
| Water Module | Digital | Water/leak detection |
| Flame sensor | Analog | Fire detection |

### Behavior

- Reads all sensors on a fixed 100ms cycle
- Packs values into a **16-byte I2C register map**
- Responds to STM32 I2C reads with current sensor state
- No logic, no decisions — pure data acquisition

### Design rationale

Arduino frees up STM32 ADC channels and demonstrates multi-MCU partitioning — a real design decision made in industrial systems.

---

## Tier 2 — STM32 Nucleo F401RE (Real-Time Controller)

This is the heart of the system. All safety logic lives here.

### FreeRTOS Tasks

| Task | Priority | Period | Job |
|------|----------|--------|-----|
| `SensorPollTask` | Normal | 100ms | I2C read from Arduino, post to queue |
| `LocalSensorTask` | Normal | 50ms | Read vibration (SW-520D), RFID, keypad directly |
| `SafetyTask` | **High** | 20ms | Check thresholds, trigger alarms, control relay/servo |
| `ActuatorTask` | Normal | 50ms | Drive stepper, servo, buzzer, LEDs based on commands |
| `DisplayTask` | Low | 200ms | Update 1602 LCD with current status |
| `CommsTask` | Normal | 500ms | Send data frame to Pi over UART |
| `WatchdogTask` | **High** | 1000ms | Feed hardware watchdog, detect task stalls |

### Safety Logic

```
FLAME detected        → relay OFF (cuts power), servo LOCK, buzzer ON, RED LED
WATER detected        → relay OFF, buzzer ON, ORANGE LED
VIBRATION spike       → log event, YELLOW LED blink, alert to Pi
TEMP > threshold      → stepper motor spins (fan simulation), alert
RFID + valid card     → servo UNLOCK door, GREEN LED
RFID + invalid card   → buzzer short beep, RED LED
Keypad PIN correct    → arm/disarm the system
Arduino silent        → FAIL-SAFE triggered (see Fail-Safe section)
```

**Critical requirement:** Actuator response to alarm must be <20ms.

### UART Protocol (STM32 ↔ Pi)

Custom framed binary protocol — not plain text:

```
[SOF 0xAA] [LEN] [MSG_TYPE] [PAYLOAD...] [CRC8] [EOF 0x55]
```

**Message types:** `SENSOR_DATA`, `ALARM_EVENT`, `STATUS_UPDATE`, `ACK`, `COMMAND`

This teaches: protocol framing, CRC calculation, state machine parsing — skills directly used in CAN, Modbus, and every industrial protocol. Document it in `docs/protocol_spec.md`.

### STM32 Peripherals Used

| Peripheral | Purpose |
|-----------|---------|
| I2C (master) | Read Arduino slave |
| UART + DMA | Non-blocking TX/RX to Pi |
| SPI | RFID RC522 |
| GPIO interrupt | Vibration sensor SW-520D, RFID IRQ |
| Timer PWM | Servo, stepper motor |
| IWDG | Hardware watchdog |
| Shift register SN74HC595 | Drive 8x8 LED matrix |
| RTC | Timestamps for events |

---

## Tier 3 — Raspberry Pi (Supervisor / HMI)

**Language:** Python

### Files

| File | Purpose |
|------|---------|
| `sentinel_daemon.py` | Serial daemon — reads UART frames, parses protocol, writes to SQLite |
| `app.py` | Flask web dashboard |
| `models.py` | SQLite data models |
| `sentinel.service` | systemd unit file — daemon starts on boot |

### Dashboard Features

- Live temperature/humidity graph (last 1 hour)
- Alarm log with timestamps
- Current system state (ARMED / DISARMED / ALARM)
- Manual override buttons (relay, servo)

### Pi Concepts Covered

- Serial communication via `pyserial`
- Protocol parsing with state machines
- SQLite for time-series data
- Flask local web app
- Linux systemd service (`systemctl enable sentinel`)
- `journalctl` debugging

---

## Fail-Safe Behavior

**Definition:** When something fails (sensor dies, cable breaks, node stops responding), the system moves to the safe state — not ignore the problem.

Rule: if a fire detector goes offline, treat it as "fire detected", not "no fire."

### Rule 1 — Arduino I2C Timeout

STM32 polls Arduino every 100ms. If I2C read fails for **3 consecutive attempts (300ms)**:

```
Arduino silent > 300ms (3 failed reads)
    → Set all Arduino sensor values to worst-case (flame=YES, water=YES, temp=MAX)
    → Trigger SYSTEM_FAULT alarm
    → Send FAULT event to Pi
    → Activate buzzer + RED LED
    → SafetyTask responds to worst-case values → relay OFF, servo LOCK
```

**Implementation inside `SensorPollTask`:**

```c
// Every successful I2C read:  i2c_fail_count = 0
// Every failed read:          i2c_fail_count++
// if (i2c_fail_count >= I2C_FAIL_LIMIT) → trigger NODE_FAULT
```

Constants (per MISRA Rule 2):
```c
#define I2C_FAIL_LIMIT  (3U)
#define I2C_POLL_MS     (100U)
```

### Rule 2 — Pi UART Silence

If Pi stops acknowledging for >5 seconds, STM32 continues operating autonomously. Safety logic **never** depends on the Pi being online. The Pi is a supervisor, not a controller.

---

## MISRA-C Rules Applied in This Project

These are a subset of the automotive MISRA-C standard (used by Bosch, Continental, STMicro).

### Rule 1 — No Dynamic Memory Allocation

```c
// WRONG
uint8_t *buffer = malloc(16);

// CORRECT
static uint8_t buffer[16];
```

No `malloc()` or `free()` anywhere in the firmware. All memory allocated statically at startup.

### Rule 2 — No Magic Numbers

```c
// WRONG
if (temperature > 60) { ... }
if (i2c_fail_count >= 3) { ... }

// CORRECT
#define TEMP_ALARM_THRESHOLD_C  (60U)
#define I2C_FAIL_LIMIT          (3U)

if (temperature > TEMP_ALARM_THRESHOLD_C) { ... }
if (i2c_fail_count >= I2C_FAIL_LIMIT) { ... }
```

### Rule 3 — Always Check Return Values

```c
// WRONG
HAL_I2C_Master_Receive(&hi2c1, addr, buf, len, timeout);

// CORRECT
if (HAL_I2C_Master_Receive(&hi2c1, addr, buf, len, timeout) != HAL_OK) {
    i2c_fail_count++;
}
```

### Rule 4 — Initialize Every Variable

```c
// WRONG
uint8_t sensor_value;
process(sensor_value);  // undefined behavior

// CORRECT
uint8_t sensor_value = 0U;
```

### Rule 5 — FreeRTOS Stack Overflow Detection

In `FreeRTOSConfig.h`:
```c
#define configCHECK_FOR_STACK_OVERFLOW  2
```

Hook implementation:
```c
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    while(1);
}
```

### Rule 6 — Explicit Integer Types

```c
// WRONG
int temperature;
char buffer[16];

// CORRECT
int32_t temperature;
uint8_t buffer[16];
```

### MISRA Summary Table

| Rule | What it means | Why |
|------|--------------|-----|
| No malloc/free | Static memory only | Predictable, no runtime failures |
| No magic numbers | Name every constant | Readable, maintainable |
| Check return values | Never ignore HAL/function errors | Catch failures before they cascade |
| Initialize variables | Set value at declaration | No undefined behavior |
| Stack overflow detection | FreeRTOS hook enabled | Catch crashes immediately |
| Explicit integer types | uint8_t not int | Portable, no size surprises |

---

## Full Component Map

| Component | Tier | Purpose |
|-----------|------|---------|
| STM32 Nucleo F401RE | Tier 2 | Real-time controller |
| Arduino Uno | Tier 1 | Field sensor node |
| Raspberry Pi | Tier 3 | Supervisor / HMI |
| DHT11 | Arduino | Temp + humidity |
| LM35 | Arduino | Analog temperature |
| Flame sensor | Arduino | Fire detection |
| Water module | Arduino | Leak detection |
| Big Sound module | Arduino | Noise detection |
| Photoresistance | Arduino | Light level |
| Vibration SW-520D x2 | STM32 | Shock / vibration (interrupt-driven) |
| RFID RC522 | STM32 | SPI, access control |
| 4x4 Keypad | STM32 | PIN entry, ARM/DISARM |
| 5V Relay | STM32 | Power cut actuator |
| SG90 Servo | STM32 | Door lock simulation |
| Stepper Motor | STM32 | Fan simulation |
| Active Buzzer | STM32 | Alarm output |
| 1602 LCD Display | STM32 | Local status display |
| 8x8 LED Matrix | STM32 via SN74HC595 | Visual alarm pattern |
| SN74HC595 shift register | STM32 | Drive 8x8 matrix |
| RGB LED | STM32 | System state indicator |
| RTC Clock Module | STM32 | Timestamps for events |
| 7-Segment Display | STM32 | Numeric display (temp readout) |
| IR Receiver | STM32 | Remote control override |

---

## Build Order

### Phase 0 — Arduino Warmup (3–4 hours, not a full week)

- Wire DHT11, LM35, flame, water, sound, photoresistance
- Read all sensors in loop, print to Serial — verify wiring
- **Stop here** — don't implement I2C slave yet
- Goal: confirm every sensor works before moving on

> This is a warmup only. Do not let it stretch beyond one day.

### Phase 1 — STM32 Standalone (2 weeks)

- Toolchain: VSCode + CMake + OpenOCD (push through the friction)
- FreeRTOS: 3 tasks blinking 3 LEDs at different rates — first RTOS experience
- UART bare metal / register level
- SPI for RFID RC522
- Keypad scan
- 1602 LCD display
- Servo + relay via PWM/GPIO

### Phase 2 — Connect Arduino via I2C (1 week)

- Arduino: implement I2C slave register map (16 bytes, one per sensor)
- STM32: bare metal I2C master driver
- Verify data integrity end-to-end: trigger sensor → see value in STM32
- Implement I2C failure detection (fail-safe counter)

### Phase 3 — Safety Logic (1 week)

- Implement all threshold logic in `SafetyTask`
- Wire vibration sensors with GPIO interrupts
- Test: trigger flame sensor → relay cuts in <20ms
- Test: unplug Arduino I2C → SYSTEM_FAULT alarm within 300ms

### Phase 4 — UART Protocol (1 week)

- Design the frame format (`protocol_spec.md`)
- Implement TX on STM32
- Implement parser on Pi (Python, state machine)
- Verify CRC end-to-end

### Phase 5 — Pi Supervisor (1 week)

- SQLite logging in `sentinel_daemon.py`
- Flask dashboard (`app.py`)
- systemd service (`sentinel.service`)
- Command sending back to STM32

### Phase 6 — Polish (1 week)

- Hardware watchdog (IWDG)
- Error handling pass
- Apply MISRA rules across all firmware files
- README with architecture diagram
- Short demo video

**Total: ~6 focused weeks.**

---

## Interview Answer

> "I built a 3-tier distributed monitoring system modeled after industrial safety controllers. The STM32 runs FreeRTOS with 7 tasks communicating via queues. It reads an Arduino I2C sensor node, drives safety actuators with deterministic response times, and communicates with a Raspberry Pi supervisor over a custom framed binary protocol. The system implements fail-safe behavior — if the sensor node stops responding, the controller detects the failure within 300ms and assumes worst-case sensor values. The Pi runs a Python daemon as a systemd service and serves a live web dashboard with historical sensor data."

---

## Key Technical Differentiators (for interviews)

1. **FreeRTOS** — real multitasking firmware with priorities and inter-task communication via queues, not a superloop
2. **Custom binary UART protocol** — framed with CRC, same structure as Modbus RTU, CANopen, J1939
3. **I2C master/slave between two MCUs** — multi-MCU system partitioning
4. **Safety-critical logic** — `SafetyTask` highest priority, deterministic, unaffected by other task hangs
5. **Fail-safe behavior** — communication failures handled; Arduino loss triggers worst-case response in <300ms
6. **Linux integration** — bare-metal firmware bridged to a full Linux application stack
7. **Hardware watchdog** — system recovers from software hangs; IWDG enabled
8. **MISRA-C practices** — no malloc, named constants, checked return values, explicit types
9. **systemd service** — production deployment on Pi, not a script run manually
