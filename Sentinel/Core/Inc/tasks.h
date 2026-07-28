#ifndef TASKS_H

#define TASKS_H

// Poll Arduino Sensor values
void SensorPollTask(void* arguments);

// Read RFID and Keypad directly on Stm32
void LocalSensorTask(void* arguments);

// Check thresholds, trigger alarms and control relay/servo
void SafetyTask(void* arguments);

// Drive fan, servo, LEDs and buzzer based on commands
void ActuatorTask(void* arguments);

// Update LCD display with current system status
void DisplayTask(void* arguments);

// Report to Pi over UART
void CommsTask(void* arguments);

// Feed hardware watchdog and detect task stalls
void WatchdogTask(void* arguments);

#endif
