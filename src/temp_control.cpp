#include <Arduino.h>
#include <DallasTemperature.h>
#include "temp_control.h"

#define LED_BLUE 2
#define LED_RED 17
#define ONE_WIRE_BUS 19


// initializing the temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Track time for each event
int currentTempSensorCount = 0;
DeviceAddress sensorAddresses[MAX_TEMP_SENSORS];
float currentTemperaturesC[MAX_TEMP_SENSORS];
float criticalTempC = 32.0;
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
  pinMode(LED_RED, OUTPUT);
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
    Serial.print("Temp ");
    Serial.print(i);
    Serial.print(": ");
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

//lights the red LED and shuts off the blue
void lightRedLED(){
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, HIGH);
}

//lights the blue LED and shuts off the red
void lightBlueLED(){
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

// lights red LED if temp is over criticalTemp else lights blue.
void lightLEDAcordingToTemp()
{
  float tempC = (currentTemperaturesC[0] + currentTemperaturesC[1] + currentTemperaturesC[2])/3;
  if(tempC > criticalTempC) lightRedLED();
  else lightBlueLED();
}