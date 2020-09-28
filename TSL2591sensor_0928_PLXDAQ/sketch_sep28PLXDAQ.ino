#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

void configureSensor(void)
{
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  // tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain (dim light)
  
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
}

//  Program entry point 
void setup(void) 
{
  Serial.begin(9600);
  configureSensor();
}

// Basic read on visible, full spectrum or infrared light (returns raw 16-bit ADC values)
void simpleRead(void)
{
  // Just read the infrared, fullspecrtrum diode or visible (difference between the two) channels.
  uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
  //uint16_t x = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2591_INFRARED);

//  Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
//  Serial.print("Luminosity: ");
   Serial.print("DATA,TIME,"); 
   Serial.println(x, DEC); 
}

// IR and Full Spectrum at once, and convert to lux
void advancedRead(void)
{
  // More advanced data read. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
//  Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
  Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
  Serial.print("Full: "); Serial.print(full); Serial.print("  ");
  Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
}

// Read using the Adafruit Unified Sensor API
void unifiedSensorAPIRead(void)
{
  // New sensor event
  sensors_event_t event;
  tsl.getEvent(&event);
 
  // Display results
//  Serial.print("[ "); Serial.print(event.timestamp); Serial.print(" ms ] ");
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    // If event.light = 0 lux the sensor is probably saturated and no reliable data could be generated
    // If event.light is +/- 4294967040 there was a float over/underflow 
    Serial.println("Invalid data (adjust gain or timing)");
  }
  else
  {
    Serial.print(event.light); Serial.println(" lux");
  }
}

// Loop
void loop(void) 
{ 
  simpleRead(); 
  // advancedRead();
  // unifiedSensorAPIRead();
  
  delay(500);
}
