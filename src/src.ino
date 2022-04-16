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
#include "Winch.h"
#include "config.h"
#include "misc.h"

Altimeter altimeter;
IMU imu;
Logger logger;
GPS gps;
Winch winch;

unsigned long prevLoopTime, currentLoopTime, startTime;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  delay(1000);

  if (!logger.begin()) {
    // Serial.print(F("logger begin failed"));  
    buzzError();
    while(1);
  }

  // Winch
  winch.begin();

  // IMU - BNO055
  if (!imu.begin())
  {
    // Serial.print(F("imu begin failed"));
    buzzError();
    while (1);
  }

  // Altimeter - BMP390
  if (!altimeter.begin()) {
    // Serial.print(F("altimeter begin failed"));
    buzzError();
    while(1);
  }

  // Init pins for cameras
  camInit();

  // GPS
  gps.begin();

 // ensure GPS has a fix
 // acquire initial GPS location
 while(!data.GPSfix) {
  buzzGPS();
  gps.sample();
 }

data.latInit = data.latCurr;
data.lonInit = data.lonCurr;
//  data.latInit = radians(36.059607);
//  data.lonInit = radians(-115.212231);

// Serial.println(F("Sensors initialized, GPS location acquired."));
buzzSuccess();
delay(1000);
}

void loop() {

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
    logger.writeData();
  } else {
    // maintain current time
    data.flightTime = data.flightTime;
  }

  /* state machine */
  switch (data.state){
    case CHECK:
      goToState(DESCENT);
      break;

    case PAD:
      // sample at ___ Hz
      imu.sample();
      altimeter.sample();
      if (imu.detectLiftoff() || altimeter.detectLiftoff())
        camOn();
        goToState(ASCENT);
      break;

    case ASCENT:
      // turn on camera
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
      winch.command();
      if (altimeter.detectLanding()) {
        camOff();
        goToState(LANDED);
      }
      break;
    
    case LANDED:
      break;
    
    case TEST:
      break;
  }
 
}
