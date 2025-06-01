void setUpLED();
void lightLEDAcordingToTemp();
void ledUpdateTask(void *pvParameters);

void printTempToUSB();
void printTempToUSBTask(void *pvParameters);

void setUpFan();
void controlFanByStratification();
void fanControlTask(void *pvParameters);

void setUpVent();
void controlVentByTemp();
void extendVent();  
void retractVent();
void stopVent();
void ventControlTask(void *pvParameters);

extern float hotTempC;
extern float coldTempC;

extern unsigned long outputUpdateIntervalMs;
extern unsigned long lastOutputReadTime;

extern float runFanTempDiffC;