#include <DallasTemperature.h>

void initTemperatureSensor();
void temperaturePollTask(void *pvParameters);
void updateTemperature();
String addressToString(DeviceAddress deviceAddress);
float getSensorTempByID(const DeviceAddress& target);

#define MAX_TEMP_SENSORS 3

extern const DeviceAddress TEMP_SENSOR_ID_TOP;
extern const DeviceAddress TEMP_SENSOR_ID_BOTTOM;
extern const DeviceAddress TEMP_SENSOR_ID_OUTSIDE;

extern int currentTempSensorCount;
extern DeviceAddress sensorAddresses[MAX_TEMP_SENSORS];
extern float currentTemperaturesC[MAX_TEMP_SENSORS];

extern unsigned long inputUpdateIntervalMs;
extern unsigned long lastInputReadTime;

extern DallasTemperature sensors;

