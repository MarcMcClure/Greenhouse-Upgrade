
#include <Arduino.h>
#include <unity.h>
#include "output_control.h"
#include "../include/test_utills.h"

// === setUpLED() ===
// Initializes LED GPIO pins; not unit tested due to hardware GPIO setup

// === lightLEDAcordingToTemp() ===
// Sets LED state based on current temperature values; not unit tested due to GPIO use

// === ledUpdateTask(void *pvParameters) ===
// FreeRTOS task that periodically updates LED state; not unit tested (task + GPIO)

// === printTempToUSB() ===
// Prints current temperatures over serial; testable with serial mocking, skipped for now

// === printTempToUSBTask(void *pvParameters) ===
// FreeRTOS task that periodically prints temps to USB; not unit tested (task + serial)


// === setUpFan() ===
// Configures fan control pin; not unit tested due to GPIO setup

// === controlFanByStratification() ===
// Turns fan on/off based on vertical temperature delta; partially testable with GPIO mocking

// === fanControlTask(void *pvParameters) ===
// FreeRTOS task to periodically apply fan control logic; not unit tested (task + GPIO)


// === setUpVent() ===
// Initializes GPIOs for linear actuator control; not unit tested (GPIO)

// === controlVentByTemp() ===
// Controls vent based on top/bottom temperature thresholds; not unit tested (GPIO logic)

// === extendVent() ===
// Sets actuator pins to extend vent; not unit tested (direct GPIO)

// === retractVent() ===
// Sets actuator pins to retract vent; not unit tested (direct GPIO)

// === stopVent() ===
// Disables actuator movement; not unit tested (GPIO control)


void setup() {
  UNITY_BEGIN();
  //litterly everything in this file is not testiable without mock hardware
  UNITY_END();
}

void loop() {}

