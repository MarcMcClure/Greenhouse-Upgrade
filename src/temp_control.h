void initTemperatureSensor();
void setUpLED();
void updateTemperature();
void printTempToUSB();
void lightRedLED();
void lightBlueLED();
void lightLEDAcordingToTemp();

extern float currentTempC;
extern float criticalTempC;
extern unsigned long updateIntervalMs;
extern unsigned long lastTempReadTime;
