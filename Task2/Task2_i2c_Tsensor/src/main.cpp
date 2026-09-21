#include <Arduino.h>
#include "DS1621.h"

DS1621 sensor(0x48); // Create an instance of the DS1621 class with the default I2C address
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensor.begin();
  sensor.configure(false);  // Configure the sensor for continuous conversion
  sensor.startConversion(); // Start temperature conversion
}

void loop()
{
  // put your main code here, to run repeatedly:
  float temperature;
  if (sensor.readTemperature(temperature) == DS1621_OK)
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }
  else
  {
    Serial.println("Error reading temperature");
  }
  delay(1000); // Wait for 1 second
}
