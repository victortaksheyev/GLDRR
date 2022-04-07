// main setup and loop for GLDRR
// for IMU (BNO055), reference read_all_data example
// for Altimeter (BMP390), reference simple_test
#include <Wire.h>
#include <Arduino.h>
#include "data.h"
#include "imu.h"
#include "altimeter.h"
#include "logger.h"
#include "GPS.h"

Altimeter altimeter;
IMU imu;
Logger logger("parktest1.txt");
GPS gps;
//(TODO) servo

unsigned long prevLoopTime, currentLoopTime, startTime;

void setup() {
  Serial.begin(115200);
  // IMU - BNO055
  if (!imu.begin())
  {
    Serial.print(F("imu begin failed"));
    while (1);
  }
  // Altimeter - BMP390
  if (!altimeter.begin()) {
    Serial.print(F("altimeter begin failed"));
    while(1);
  }
  // Logger
//  if (!logger.begin()) {
//    Serial.print(F("logger begin failed"));  
//    digitalWrite(LED_BUILTIN, HIGH);
//    while(1);
//  }
  // GPS
  gps.begin();

 // TODO:
 // ensure GPS has a fix
 // ensure IMU is calibrated (store calibrated values in EEPROM)
   /*
  while (!imu.calibrated()) {
    imu.calibrate();
    Serial.println(F("IMU calibrating"));
  }
  */

  Serial.println("Adafruit GPS library basic parsing test!");

  delay(1000);
}

void loop() {
  Serial.print(data.flightTime); Serial.print("|");Serial.print(data.GPSfix);Serial.print("|");Serial.print(data.latCurr); Serial.print("|");Serial.print(data.lonCurr); 
  Serial.print("|"); Serial.print(data.heading); Serial.print("|");Serial.print(gps.calcDistance());Serial.println("|");  

  /* sensor timing */
  currentLoopTime = micros();
  data.delta_t = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;
  
  /* flight timing */
  if (data.state <= PAD){
    data.flightTime = 0.0;
    startTime = millis();
  } else if (data.state > PAD && data.state < LANDED) {
    data.flightTime = float((millis() - startTime) / 1000.0f);
//    logger.writeData();
  } else {
    // maintain current time
    data.flightTime = data.flightTime;
  }

  /* state machine */
  switch (data.state){
    case CHECK:
      goToState(PAD);
      break;

    case PAD:
      // sample at ___ Hz
      imu.sample();
      altimeter.sample();
      if (imu.detectLiftoff() || altimeter.detectLiftoff())
        goToState(ASCENT);
      break;

    case ASCENT:
      imu.sample();
      altimeter.sample();
      if (altimeter.detectApogee())
        goToState(APOGEE);
      break;
    
    case APOGEE:
      goToState(DESCENT);
      break;
    
    case DESCENT:
      gps.sample();
      gps.calcBearing();
      imu.sample();
      altimeter.sample();
      
      if (true)
        goToState(LANDED);
      break;
    
    case LANDED:
      break;
    
    case TEST:
      break;
  }
 
}
