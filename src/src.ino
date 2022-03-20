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
  while (!imu.calibrated) {
    imu.calibrate();
    Serial.println("IMU calibrating");
  }

  // ensure GPS has a fix

  // sensor timing
  currentLoopTime = micros();
  data.delta_t = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;

  Serial.print("State: "); Serial.println(data.state);
  Serial.print("Altitude: ");Serial.println(altimeter.altitude);

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
    case IDLE:
      Serial.println("IDLE");
      goToState(LIFTOFF);
      break;

    case LIFTOFF:
      Serial.println("LIFTOFF");
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






















/*


void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("Accl:\t");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:\t");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    Serial.print("Mag:\t");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    Serial.print("Gyro:\t");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    Serial.print("Rot:\t");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    Serial.print("Linear:\t");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    Serial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    Serial.print("Unk:\t");
  }

  Serial.print("\tx= ");
  Serial.print(x);
  Serial.print(" |\ty= ");
  Serial.print(y);
  Serial.print(" |\tz= ");
  Serial.println(z);
}


void printIMUCalibration() {
//  bno.getCalibration(&sysCal, &gyroCal, &accelCal, &magCal);
  Serial.println();
  Serial.print("Calibration: Sys=");
  Serial.print(sysCal);
  Serial.print(" Gyro=");
  Serial.print(gyroCal);
  Serial.print(" Accel=");
  Serial.print(accelCal);
  Serial.print(" Mag=");
  Serial.println(magCal);


}
  */
