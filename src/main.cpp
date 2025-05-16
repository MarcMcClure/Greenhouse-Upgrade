/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

// Set LED_BUILTIN if it is not defined by Arduino framework
#define LED_BUILTIN 2

void testCalculationAndOutput();
void flashLED();

void setup()
{
  testCalculationAndOutput();
}


void loop()
{

}

// do a caluclation then print the variables and output
void testCalculationAndOutput()
{
  //wait for USB port to connect
  Serial.begin(9600); //set to default moniter speed
  while (!Serial);

  int a = 7;
  int b = 5;
  int result = a + b;

  Serial.println("Simple Calculation:");
  Serial.print(a);
  Serial.print(" + ");
  Serial.print(b);
  Serial.print(" = ");
  Serial.println(result);
}

// function to flash an LED
// setup by initializeing LED digital pin as an output with pinMode(LED_BUILTIN, OUTPUT);
void flashLED()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);

  // wait for a second
  delay(1000);

  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);

   // wait for a second
  delay(1000);
}