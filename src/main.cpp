/**
 * lights red LED when sensor is warm, blue when its cold, and prints temp to serial moniter every second
 */
#include "Arduino.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>

#include "wifi_setup.h"
#include "temp_control.h"
#include "http_routes.h"

WebServer server(80); // HTTP server on port 80

void setup()
{
  Serial.begin(9600);
  setUpLED();
  startWiFi();
  initTemperatureSensor();

  registerRoutes(server);
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