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
float currentTempC = 0.0;
float criticalTempC = 32.0;
unsigned long updateIntervalMs = 1000;
unsigned long lastTempReadTime = 0;

void initTemperatureSensor(){
      sensors.begin();
}

void setUpLED(){
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void updateTemperature() {
  sensors.requestTemperatures();
  currentTempC = sensors.getTempCByIndex(0);
}

void printTempToUSB(){
    float tempC = currentTempC;
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.println(" °C");
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
  float tempC = currentTempC;
  if(tempC > criticalTempC) lightRedLED();
  else lightBlueLED();
}