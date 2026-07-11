// Includes
#include <DHT.h>

// Definitions
// DHT Sensor
#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LM35 Sensor
#define LM35_PIN A0

// Big Sound Module
#define SOUND_PIN 3

// Water Sensor
#define WATER_PIN A2

// Fire Sensor
#define FIRE_PIN A3

// Photo Resistance 
#define PHOTO_RESISTANCE_PIN A4

// Private variables
uint8_t dht_temperature = 0U;
uint8_t dht_humidity = 0U;
uint8_t LM35_temperature = 0U;
uint16_t sound_value = 0U;
uint16_t water_value = 0U;
uint16_t fire_value = 0U;
uint16_t light_value = 0U;

// Helpers
uint8_t measure_LM35_temperature(){
  // Get a reading from the temperature sensor
  uint16_t LM35_reading = analogRead(LM35_PIN);

  // Convert the reading into Voltage (mV)
  float voltage = LM35_reading * (5000.0f / 1024.0f);

  // Convert voltage into temperature (°C)
  uint8_t temperature = (uint8_t)(voltage / 10);

  return temperature;
}

uint16_t measure_sound_value(){
  return digitalRead(SOUND_PIN);
}

uint16_t measure_water_value(){
  return analogRead(WATER_PIN);
}

uint16_t measure_fire_value(){
  return analogRead(FIRE_PIN);
}

uint16_t measure_light_value(){
  return analogRead(PHOTO_RESISTANCE_PIN);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  dht_temperature = dht.readTemperature();
  dht_humidity = dht.readHumidity();
  LM35_temperature = measure_LM35_temperature();
  sound_value = measure_sound_value();
  water_value = measure_water_value();
  fire_value = measure_fire_value();
  light_value = measure_light_value();


  Serial.print("DHT Temperature: ");
  Serial.println(dht_temperature);
  Serial.print("DHT Humidity: ");
  Serial.println(dht_humidity);

  Serial.print("LM35 temp ");
  Serial.println(LM35_temperature);

  Serial.print("Sound: ");
  Serial.println(sound_value);

  Serial.print("Water: ");
  Serial.println(water_value);

  Serial.print("Fire: ");
  Serial.println(fire_value);

  Serial.print("Light: ");
  Serial.println(light_value);

  Serial.println("--------------------------------------");
  delay(1000);
}
