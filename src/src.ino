// main setup and loop for GLDRR
// for IMU (BNO055), reference read_all_data example
// for Altimeter (BMP390), reference simple_test
#include <Wire.h>
#include "data.h"
#include "imu.h"
#include "altimeter.h"

Altimeter altimeter;
IMU imu;
//(TODO) GPS
//(TODO) SD logger
//(TODO) servo

unsigned long prevLoopTime, currentLoopTime, startTime;

void setup() {
  Serial.begin(115200);

  // IMU - BNO055
  if (!imu.begin())
  {
    Serial.print("no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  // Altimeter - BMP390
  if (!altimeter.begin()) {
    Serial.print("no BMP390 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
  // ensure IMU is calibrated
  while (!imu.calibrated()) {
    imu.calibrate();
    Serial.println("IMU calibrating");
  }

  // ensure GPS has a fix

  // sensor timing
  currentLoopTime = micros();
  data.delta_t = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;

  Serial.print("State: "); Serial.println(data.state);
  Serial.print("Altitude: ");Serial.println(data.altitude);
  

  // flight timestamp
  if (data.state <= PAD){
    data.flightTime = 0.0;
    startTime = millis();
  } else if (data.state > PAD && data.state < LANDED) {
    data.flightTime = float((millis() - startTime) / 1000.0f);
  } else {
    // maintain current time
    data.flightTime = data.flightTime;
  }
 
  // functions to run every loop
  imu.sample();
  altimeter.sample();

  // state machine
  switch (data.state){
    case CHECK:
      Serial.println("IDLE");
      goToState(PAD);
      break;

    case PAD:
      Serial.println("PAD");
      if (imu.detectLiftoff() || altimeter.detectLiftoff())
        goToState(ASCENT);
      break;

    case ASCENT:
      if (altimeter.detectApogee())
        goToState(APOGEE);
      break;
    
    case APOGEE:
      goToState(DESCENT);
      break;
    
    case DESCENT:
      break;
    
    case LANDED:
      break;
    
    case ABORT:
      break;
    
    case TEST:
      break;
  }

}
