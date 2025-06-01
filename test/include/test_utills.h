#pragma once
#include <Arduino.h>
#include "input_control.h"

static const DeviceAddress mockTop     = {0x28, 0x69, 0x76, 0x54, 0x44, 0x24, 0x0b, 0x19};
static const DeviceAddress mockBottom  = {0x28, 0x15, 0x2A, 0x59, 0x44, 0x24, 0x0B, 0xD5};
static const DeviceAddress mockOutside = {0x28, 0x13, 0x09, 0x52, 0x44, 0x24, 0x0B, 0x45};
static const DeviceAddress badSensorID = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00, 0x00};

inline void setup_mock_sensor_data() {
  currentTempSensorCount = 3;
  memcpy(sensorAddresses[0], mockTop, sizeof(DeviceAddress));
  memcpy(sensorAddresses[1], mockBottom, sizeof(DeviceAddress));
  memcpy(sensorAddresses[2], mockOutside, sizeof(DeviceAddress));

  currentTemperaturesC[0] = 25.5;
  currentTemperaturesC[1] = 19.0;
  currentTemperaturesC[2] = 12.7;
}