#include <DallasTemperature.h>

void initTemperatureSensor();
void setUpLED();
void updateTemperature();
void printTempToUSB();
void lightRedLED();
void lightBlueLED();
void lightLEDAcordingToTemp();
String addressToString(DeviceAddress deviceAddress);

#define MAX_TEMP_SENSORS 3

extern int currentTempSensorCount;
extern DeviceAddress sensorAddresses[MAX_TEMP_SENSORS];
extern float currentTemperaturesC[MAX_TEMP_SENSORS];
extern float criticalTempC;
extern unsigned long updateIntervalMs;
extern unsigned long lastTempReadTime;
