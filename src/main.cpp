/**
 * lights red LED when sensor is warm, blue when its cold, and prints temp to serial moniter every second
 */
#include "Arduino.h"
#include "credentials.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>

// Define GPIO pins
#define LED_BLUE 2
#define LED_RED 17
#define ONE_WIRE_BUS 19 //temp sensor

void setUpLED();
void startWiFi();
void lightRedLED();
void lightBlueLED();
void lightLEDAcordingToTemp();
void printTempToUSB();
void updateTemperature();
void handleGetTemp();

WebServer server(80); // HTTP server on port 80

// initializing the temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Track time for each event
unsigned long lastLEDUpdate = 0;
unsigned long lastTempReadTime = 0;

float currentTempC = 0.0;
float criticalTempC = 32.0;
unsigned long updateIntervalMs = 1000;  // default sensor update interval 1 second

void setup()
{
  Serial.begin(9600);
  setUpLED();
  startWiFi();
  sensors.begin();

  server.on("/GETtemp", handleGetTemp);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  unsigned long currentTime = millis();
  

  //print temp is it had been over 1 second since the last time the temp was printed
  if (currentTime - lastTempReadTime >= updateIntervalMs){
    updateTemperature();
    printTempToUSB();
    lightLEDAcordingToTemp();
    lastTempReadTime = currentTime;
  }

  server.handleClient();

}

void setUpLED(){
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void startWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void updateTemperature() {
  sensors.requestTemperatures();
  currentTempC = sensors.getTempCByIndex(0);
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

void printTempToUSB(){
    float tempC = currentTempC;
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.println(" °C");
}

void handleGetTemp() {
  String json = "{ \"temp_c\": " + String(currentTempC, 2) + " }";
  server.send(200, "application/json", json);
}