#include <Arduino.h>
#include <unity.h>

void test_sanity_true_is_true() {
  TEST_ASSERT_TRUE(true);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_sanity_true_is_true);
  UNITY_END();
}

void loop() {}