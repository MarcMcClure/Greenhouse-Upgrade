#include <Arduino.h>
#include <DallasTemperature.h>
#include "temp_control.h"

#define LED_BLUE 15
#define LED_GREEN 0
#define LED_RED 16
#define ONE_WIRE_BUS 5
#define FAN_PIN 19
#define LINEAR_ACUTUATOR_EXTEND 32
#define LINEAR_ACUTUATOR_RETRACT 33

const DeviceAddress TEMP_SENSOR_ID_TOP = { 0x28, 0x69, 0x76, 0x54, 0x44, 0x24, 0x0b, 0x19 };
const DeviceAddress TEMP_SENSOR_ID_BOTTOM = { 0x28, 0x15, 0x2A, 0x59, 0x44, 0x24, 0x0B, 0xD5 };
const DeviceAddress TEMP_SENSOR_ID_OUTSIDE = { 0x28, 0x13, 0x09, 0x52, 0x44, 0x24, 0x0B, 0x45 };

// initializing the temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Track time for each event
int currentTempSensorCount = 0;
DeviceAddress sensorAddresses[MAX_TEMP_SENSORS];
float currentTemperaturesC[MAX_TEMP_SENSORS];
float hotTempC = 25.0;
float coldTempC = 10.0;
float runFanTempDiffC = 3.0;
unsigned long updateIntervalMs = 1000;
unsigned long lastTempReadTime = 0;

void initTemperatureSensor(){
  sensors.begin();
  currentTempSensorCount = sensors.getDeviceCount();
  for (int i = 0; i < currentTempSensorCount && i < MAX_TEMP_SENSORS; i++) {
    if (sensors.getAddress(sensorAddresses[i], i)) {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(" address: ");
      Serial.println(addressToString(sensorAddresses[i]));
    } else {
      Serial.print("Failed to get address for sensor ");
      Serial.println(i);
    }
  }
}

void setUpLED(){
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void setUpFan(){
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW); // Turn off the fan initially
}

void setUpVent(){
  pinMode(LINEAR_ACUTUATOR_EXTEND, OUTPUT);
  pinMode(LINEAR_ACUTUATOR_RETRACT, OUTPUT);
  digitalWrite(LINEAR_ACUTUATOR_EXTEND, LOW); // Turn off the actuator initially
  digitalWrite(LINEAR_ACUTUATOR_RETRACT, LOW); // Turn off the actuator initially
}

void updateTemperature() {
  sensors.requestTemperatures();
  for (int i = 0; i < currentTempSensorCount && i < MAX_TEMP_SENSORS; i++) {
      currentTemperaturesC[i] = sensors.getTempCByIndex(i);
  }
}

void printTempToUSB(){
  for (int i = 0; i < currentTempSensorCount && i < MAX_TEMP_SENSORS; i++) {
    float tempC = currentTemperaturesC[i];
    String label;

    if (memcmp(sensorAddresses[i], TEMP_SENSOR_ID_TOP, 8) == 0) {
      label = "Top";
    } else if (memcmp(sensorAddresses[i], TEMP_SENSOR_ID_BOTTOM, 8) == 0) {
      label = "Bottom";
    } else if (memcmp(sensorAddresses[i], TEMP_SENSOR_ID_OUTSIDE, 8) == 0) {
      label = "Outside";
    } else {
      label = "Unknown";
    }

    Serial.print("Temp ");
    Serial.print(label);
    Serial.print(": ");
    Serial.print(addressToString(sensorAddresses[i]));
    Serial.print(", ");
    Serial.print(tempC);
    Serial.print(" °C\t");
  }
  Serial.print("\n");
}

String addressToString(DeviceAddress deviceAddress) {
  String result = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) result += "0";
    result += String(deviceAddress[i], HEX);
  }
  return result;
}

// lights red LED if temp is over criticalTemp else lights blue.
void lightLEDAcordingToTemp() {
  float tempTop = sensors.getTempC(TEMP_SENSOR_ID_TOP);
  float tempBottom = sensors.getTempC(TEMP_SENSOR_ID_BOTTOM);

  bool tooHot = tempTop > hotTempC || tempBottom > hotTempC;
  bool tooCold = tempTop < coldTempC || tempBottom < coldTempC;

  if (tooHot) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, HIGH);
  } else if (tooCold) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, HIGH);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

void controlFanByStratification() {
  float tempTop = sensors.getTempC(TEMP_SENSOR_ID_TOP);
  float tempBottom = sensors.getTempC(TEMP_SENSOR_ID_BOTTOM);

  if (abs(tempTop - tempBottom) > runFanTempDiffC) {
    digitalWrite(FAN_PIN, HIGH); // turn fan on
  } else {
    digitalWrite(FAN_PIN, LOW); // turn fan off
  }
}

void extendVent() {
  digitalWrite(LINEAR_ACUTUATOR_RETRACT, LOW);  // Prevent short
  digitalWrite(LINEAR_ACUTUATOR_EXTEND, HIGH);
}

void retractVent() {
  digitalWrite(LINEAR_ACUTUATOR_EXTEND, LOW);   // Prevent short
  digitalWrite(LINEAR_ACUTUATOR_RETRACT, HIGH);
}

void stopVent() {
  digitalWrite(LINEAR_ACUTUATOR_EXTEND, LOW);
  digitalWrite(LINEAR_ACUTUATOR_RETRACT, LOW);
}

void controlVentByTemp() {
  // float tempTop = sensors.getTempC(TEMP_SENSOR_ID_TOP);
  // float tempBottom = sensors.getTempC(TEMP_SENSOR_ID_BOTTOM);

  // if (tempTop > hotTempC) {
  //   extendVent();
  // } else if (tempBottom < coldTempC) {
  //   retractVent();
  // } else {
  //   stopVent();
  // }
}