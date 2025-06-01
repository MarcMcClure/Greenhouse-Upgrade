#include <Arduino.h>
#include <unity.h>
#include <WebServer.h>
#include "http_routes.h"
#include "../include/test_utills.h"

WebServer testServer(80);  // Local instance for testing

// === registerRoutes() ===
void test_registerRoutes_does_not_crash() {
  registerRoutes(testServer);
  TEST_PASS_MESSAGE("registerRoutes() executed without crashing.");
}

// === handleGetSettings() ===
// cant unit test, needs clients for intigration testing

// === handleGetData() ===
// cant unit test, needs clients for intigration testing

// === handlePutSettings() ===
// requires intigration tests to verify responce is correct

// unit testing to see if a put request appropriately updates settings
// void test_handlePutSettings_updates_settings() {
//   // Simulate JSON input
//   String json = "{\"inputInterval\":5555,\"hotTempC\":25,\"coldTempC\":10,\"runFanTempDiffC\":2,\"outputInterval\":6666}";
//   testServer.setArg("plain", json);

//   // Call the handler
//   handlePutSettings(testServer);

//   // Check if the values were updated
//   TEST_ASSERT_EQUAL_UINT32(8000, inputUpdateIntervalMs);
//   TEST_ASSERT_EQUAL_UINT32(8000, outputUpdateIntervalMs);
//   TEST_ASSERT_FLOAT_WITHIN(0.001, 30.5, hotTempC);
//   TEST_ASSERT_FLOAT_WITHIN(0.001, 15.2, coldTempC);
//   TEST_ASSERT_FLOAT_WITHIN(0.001, 15.2, runFanTempDiffC);
// }

// === httpServerTask() ===
// not independently testiable

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_registerRoutes_does_not_crash);
  RUN_TEST(test_handlePutSettings_updates_settings);
  UNITY_END();
}

void loop() {}