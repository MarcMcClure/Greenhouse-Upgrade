#include <Arduino.h>
#include "output_control.h"
#include "input_control.h"

#define LED_BLUE 15
#define LED_GREEN 0
#define LED_RED 16
#define FAN_PIN 32
#define LINEAR_ACUTUATOR_EXTEND 26
#define LINEAR_ACUTUATOR_RETRACT 27
float hotTempC = 25.0;
float coldTempC = 15.0;
float runFanTempDiffC = 3.0;

unsigned long outputUpdateIntervalMs = 1000;
unsigned long lastOutputReadTime = 0;

void ledUpdateTask(void *pvParameters) {
  while (true) {
    lightLEDAcordingToTemp();
    vTaskDelay(outputUpdateIntervalMs / portTICK_PERIOD_MS);  // ~4Hz
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
  stopVent(); // Ensure the vent is stopped initially
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
    Serial.print(" °C\t\t");
  }
  Serial.print("\n");
}

void printTempToUSBTask(void *pvParameters) {
  while (true) {
    printTempToUSB();
    vTaskDelay(outputUpdateIntervalMs / portTICK_PERIOD_MS);
  }

}

// lights red LED if temp is over criticalTemp else lights blue.
void lightLEDAcordingToTemp() {

  float tempTop = getSensorTempByID(TEMP_SENSOR_ID_TOP);
  float tempBottom = getSensorTempByID(TEMP_SENSOR_ID_BOTTOM);

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
  float tempTop = getSensorTempByID(TEMP_SENSOR_ID_TOP);
  float tempBottom = getSensorTempByID(TEMP_SENSOR_ID_BOTTOM);

  if (abs(tempTop - tempBottom) > runFanTempDiffC) {
    digitalWrite(FAN_PIN, HIGH); // turn fan on
  } else {
    digitalWrite(FAN_PIN, LOW); // turn fan off
  }
}

void fanControlTask(void *pvParameters) {
  while (true) {
    controlFanByStratification();
    vTaskDelay(outputUpdateIntervalMs / portTICK_PERIOD_MS);  // Match other output tasks
  }
}

void ventControlTask(void *pvParameters) {
  while (true) {
    controlVentByTemp();
    vTaskDelay(outputUpdateIntervalMs / portTICK_PERIOD_MS);  // Match other output tasks
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
  float tempTop = getSensorTempByID(TEMP_SENSOR_ID_TOP);
  float tempBottom = getSensorTempByID(TEMP_SENSOR_ID_BOTTOM);

  if (max(tempTop, tempBottom) > hotTempC) {
    extendVent();
  } else if (min(tempTop, tempBottom) < coldTempC) {
    retractVent();
  } else {
    stopVent();
  }
}