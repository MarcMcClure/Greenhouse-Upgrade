#include <DallasTemperature.h>

void initTemperatureSensor();
void setUpLED();
void setUpFan();
void setUpVent();
void updateTemperature();
void printTempToUSB();
void lightLEDAcordingToTemp();
void controlFanByStratification();
void controlVentByTemp();
void extendVent();  
void retractVent();
void stopVent();
String addressToString(DeviceAddress deviceAddress);

#define MAX_TEMP_SENSORS 3

extern int currentTempSensorCount;
extern DeviceAddress sensorAddresses[MAX_TEMP_SENSORS];
extern float currentTemperaturesC[MAX_TEMP_SENSORS];
extern float hotTempC;
extern float coldTempC;
extern float runFanTempDiffC;
extern unsigned long updateIntervalMs;
extern unsigned long lastTempReadTime;
