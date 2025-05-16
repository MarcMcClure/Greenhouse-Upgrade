/**
 * lights red LED when sensor is warm, blue when its cold, and prints temp to serial moniter every second
 */
#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Define GPIO pins
#define LED_BLUE 2
#define LED_RED 17
#define ONE_WIRE_BUS 19 //temp sensor

void setUpLED();
void lightRedLED();
void lightBlueLED();
void lightLEDAcordingToTemp();
void printTempToUSB();

// initializing the temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Track time for each event
unsigned long lastLEDUpdate = 0;
unsigned long lastTempReadTime = 0;

void setup()
{
  setUpLED();
  Serial.begin(9600);
  sensors.begin();
}


void loop()
{
  unsigned long currentTime = millis();

  //print temp is it had been over 1 second since the last time the temp was printed
  if (currentTime - lastTempReadTime >= 1000){
    printTempToUSB();
    lastTempReadTime = currentTime;
  }

  //update LEDs, red or blue, every 0.1s
  if (currentTime - lastLEDUpdate >= 100){
    lightLEDAcordingToTemp();
    lastLEDUpdate = currentTime;
  }


}

void setUpLED(){
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
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

// lights red LED if temp is over 32C else lights blue.
void lightLEDAcordingToTemp()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if(tempC > 32) lightRedLED();
  else lightBlueLED();
}

void printTempToUSB(){
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.println(" °C");
}