#include <Arduino.h>
#include <unity.h>
#include "input_control.h"
#include "../include/test_utills.h"


// === getSensorTempByID() ===

void test_getSensorTempByID_valid_address_returns_value() {
  setup_mock_sensor_data();
  float tempTop = getSensorTempByID(mockTop);  // Should be 25.5
  float tempBottom = getSensorTempByID(mockBottom);  // Should be 19.0
  float tempOutside = getSensorTempByID(mockOutside);  // Should be 12.7
  TEST_ASSERT_EQUAL_FLOAT(25.5, tempTop);
  TEST_ASSERT_EQUAL_FLOAT(19.0, tempBottom);
  TEST_ASSERT_EQUAL_FLOAT(12.7, tempOutside);
}

void test_getSensorTempByID_invalid_address_returns_NaN() {
  setup_mock_sensor_data();
  float temp = getSensorTempByID(badSensorID);  // Invalid address
  TEST_ASSERT_TRUE(isnan(temp));
}

// === addressToString() ===

void test_addressToString_formats_correctly() {
    setup_mock_sensor_data();
    DeviceAddress mockTopMutableCopy;
    memcpy(mockTopMutableCopy, (const void*)mockTop, sizeof(DeviceAddress));
    String str = addressToString(mockTopMutableCopy);
    TEST_ASSERT_EQUAL_STRING("2869765444240b19", str.c_str());
}

// === updateTemperature() ===
// Skipped from unit testing due to direct hardware interaction (sensor reads via OneWire/Dallas)


// === initTemperatureSensor() ===
// Not unit tested: initializes hardware interfaces and depends on physical sensor presence

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_getSensorTempByID_valid_address_returns_value);
  RUN_TEST(test_getSensorTempByID_invalid_address_returns_NaN);
  RUN_TEST(test_addressToString_formats_correctly);
  UNITY_END();
}

void loop() {}






