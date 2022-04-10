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
//#include "buzzer.cpp"

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
    // Logger
  pinMode(CHIP_SELECT, OUTPUT);
  digitalWrite(CHIP_SELECT, HIGH);

  if (!logger.begin()) {
//    Serial.print(F("logger begin failed"));  
    buzzError();
    while(1);
  }

  // ensure regulator ON
//  pinMode(REG_ENABLE, OUTPUT);
//  digitalWrite(REG_ENABLE, HIGH);

  // Winch
  winch.begin();

  // IMU - BNO055
  if (!imu.begin())
  {
//    Serial.print(F("imu begin failed"));
    buzzError();
    while (1);
  }
//   Altimeter - BMP390
  if (!altimeter.begin()) {
//    Serial.print(F("altimeter begin failed"));
    buzzError();
    while(1);
  }

  // GPS
  gps.begin();

 // ensure GPS has a fix
 // acquire initial GPS location
 while(!data.GPSfix) {
  gpsBuzz();
  gps.sample();
 }
// data.latInit = data.latCurr;
// data.lonInit = data.lonCurr;
data.latInit = radians(36.059607);
data.lonInit = radians(-115.212231);

  
//  while (!imu.calibrated()) {
//    imu.calibrate();
//    Serial.println(F("IMU calibrating"));
//    delay(20);
//  }

//  Serial.println(F("Sensors initialized, GPS location acquired."));
  buzzSuccess();
  delay(1000);
}

void loop() {
//  Serial.print(data.flightTime); Serial.print("|");Serial.print(data.GPSfix);Serial.print("|");Serial.print(data.latCurr); Serial.print("|");Serial.print(data.lonCurr); 
//  Serial.print("|"); Serial.print(data.heading); Serial.print("|");Serial.print(gps.calcDistance());Serial.println("|");  

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
      if (altimeter.detectLiftoff())
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
      winch.command();
      
//      if (true)
//        goToState(LANDED);
      break;
    
    case LANDED:
      break;
    
    case TEST:
      break;
  }
 
}

void buzzError() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 200);
    delay(500);
    noTone(BUZZER);
    delay(100);
  }
}


void buzzSuccess() {
    // pulsing for flight
    for (int i = 0; i < 3; i++) {
      tone(BUZZER, 1000);
      delay(200);
      tone(BUZZER, 2000);
      delay(500);
    }
   
    noTone(BUZZER);
}

void gpsBuzz() {
    tone(BUZZER, 1000);
    delay(20);
    noTone(BUZZER);
}
