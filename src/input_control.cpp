#include <Arduino.h>
#include <DallasTemperature.h>
#include "input_control.h"


#define ONE_WIRE_BUS 5

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
unsigned long inputUpdateIntervalMs = 1000;
unsigned long lastInputReadTime = 0;

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

void temperaturePollTask(void *pvParameters){
  while (true) {
    updateTemperature();
    vTaskDelay(inputUpdateIntervalMs / portTICK_PERIOD_MS);  // ~4Hz
  }
}

void updateTemperature() {
sensors.requestTemperatures();
  for (int i = 0; i < currentTempSensorCount && i < MAX_TEMP_SENSORS; i++) {
      currentTemperaturesC[i] = sensors.getTempCByIndex(i);
  }
}

String addressToString(DeviceAddress deviceAddress) {
  String result = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) result += "0";
    result += String(deviceAddress[i], HEX);
  }
  return result;
}

float getSensorTempByID(const DeviceAddress& target) {
  for (int i = 0; i < currentTempSensorCount; ++i) {
    if (memcmp(sensorAddresses[i], target, sizeof(DeviceAddress)) == 0) {
      return currentTemperaturesC[i];
    }
  }
  return NAN; // Return Not-a-Number if not found
}