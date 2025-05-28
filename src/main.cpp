/**
 * lights red LED when sensor is warm, blue when its cold, and prints temp to serial moniter every second
 */
#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>

#include "wifi_setup.h"
#include "output_control.h"
#include "input_control.h"
#include "http_routes.h"

void setup()
{
  Serial.begin(9600);
  setUpLED();
  setUpFan();
  setUpVent();
  startWiFi();
  initTemperatureSensor();

  registerRoutes(server);
  server.begin();
  Serial.println("HTTP server started");

  xTaskCreate(
    ledUpdateTask,        // Task function
    "LED Update",         // Name
    1024,                 // Stack size in words (not bytes)
    NULL,                 // Task input parameter
    1,                    // Priority
    NULL                  // Task handle (not used)
  );

  xTaskCreate(
    temperaturePollTask,
    "Temperature Poll",
    1024,                 // may need to be larger TODO:test memory usage
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    printTempToUSBTask,
    "USB Temp Print",
    2048,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    fanControlTask,
    "Fan Control",
    1024,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    httpServerTask,
    "HTTP Server",
    2048,  // Give it room for handling requests
    NULL,
    1,
    NULL
);
}

void loop()
{
  // The loop function is intentionally left empty as all tasks are handled in FreeRTOS tasks.
  // This allows the ESP32 to run multiple tasks concurrently.
  vTaskDelay(1000 / portTICK_PERIOD_MS); // Prevent watchdog timer reset
}