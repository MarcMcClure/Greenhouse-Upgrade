#include <Arduino.h>
#include <unity.h>
#include <WiFi.h>
#include "wifi_setup.h"
#include "../include/test_utills.h"

// Test that startWiFi() runs without crashing
void test_wifi_setup() {
  startWiFi();  // Should not crash or hang
  TEST_PASS();  // Always passes unless it crashes
}

// Test that WiFi is actually connected after calling startWiFi()
void test_wifi_connected() {
  TEST_ASSERT_TRUE(WiFi.isConnected());  // Check ESP's connection status
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_wifi_setup);
  RUN_TEST(test_wifi_connected);
  UNITY_END();
}

void loop() {}