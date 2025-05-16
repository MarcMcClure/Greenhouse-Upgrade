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
void swapLED();
void printTempToUSB();

// initializing the temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Track time for each event
unsigned long lastLEDSwapTime = 0;
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

  //swap LEDs if more than a second and a half has elapsed since the last swap
  if (currentTime - lastLEDSwapTime >= 1500){
    swapLED();
    lastLEDSwapTime = currentTime;
  }

  if (currentTime - lastTempReadTime >= 1000){
    printTempToUSB();
    lastTempReadTime = currentTime;
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

// function to swap whigh LED is lit
void swapLED()
{
  if(digitalRead(LED_BLUE) == HIGH) lightRedLED();
  else lightBlueLED();
}

void printTempToUSB(){
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.println(" °C");
}