// main setup and loop for GLDRR
// for IMU (BNO055), reference read_all_data example
// for Altimeter (BMP390), reference simple_test
#include <Wire.h>
#include "data.h"
#include "imu.h"
#include "altimeter.h"


Altimeter altimeter;
IMU imu;

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

  // Timing
  currentLoopTime = micros();
  data.delta_t = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;

  /* */

  Serial.print("State: "); Serial.println(data.state);
  Serial.print("Altitude: ");Serial.println(altimeter.altitude);

  // this is the flight timestamp
  if (data.state < LIFTOFF)
  {
    data.flightTime = 0.0;
    startTime = millis();
  }
  else if (data.state > LIFTOFF || data.state < LANDED)
  {
    data.flightTime = float((millis() - startTime) / 1000.0f);
  }
  else if (data.state == LANDED)
  {
    data.flightTime = data.flightTime;
  }

  // functions to run every loop
  if (imu.calibrated()) imu.sample(); // updates the IMU when new data is available
  altimeter.sample();                 // updates the barometric sensor when new data is available

  switch (data.state)
  {
  case IDLE:
    // Checks continuity every (check config.h for timer)
    // do stuffs
    goToState(LIFTOFF);
    break;

  case LIFTOFF:
#if is_DEBUG
    Serial.println("LIFTOFF!");
#endif
    if ((imu.accel.z - 9.8067f) >= 3.0f || altimeter.altitude >= 1.0f)
    {
      // if (liftoffTimer.hasPassed(LAUNCH_ACCEL_TIME_THRESHOLD)) // noise safe timer so liftoff is only detect if a accel threshold is exceded for some particular time
        goToState(POWERED_ASCENT);
    }
    break;

  case POWERED_ASCENT:
#if is_DEBUG
    Serial.println("POWERED_ASCENT!");
#endif
    // FIXME
    // calculate total accel of the vehicle = sqrt(x^2 + y^2 + z^2)
    float accelTotal;
    accelTotal = (sqrt(sq(imu.accel.x) + sq(imu.accel.y) + sq(imu.accel.z)));
    if (accelTotal < 1.5f)
      goToState(MECU);
    break;
  case MECU:
    if (altimeter.detectApogee())
      goToState(APOGGE);
    break;
  case APOGGE:
#if is_DEBUG
    Serial.println("APOGGE!");
#endif
    // if (pyro.fire())
    goToState(PARACHUTE_DESCENT);
    break;
  case PARACHUTE_DESCENT:
#if is_DEBUG
    Serial.println("PARACHUTE_DESCENT!");
#endif
    // pyro.allOff(); // just to make sure the pyro channels go off
    // TODO find a better way
    // if (data.altimeter.altitude <= LIFTOFF_ALTITUDE_THRESHOLD) // this might not work if we stay above the pre-set AGL altitute, then the threshold will never be reached and the system will be kept in this state: to mitigate this we could check the rocket attitute to see if its stationary or not
      // goToState(LANDED);
    break;
  case LANDED:
// BEEP and blink LED: make it easier to find!
#if is_DEBUG
    Serial.println("LANDED!");
#endif
//    led.blinkSlow();
    break;

  case ABORT:
    // not much we could do besides firing the parachute...
    break;

  case TEST: // debug mode

    break;
  }

#if is_RADIO
  telemetry.send2radio();
#endif

#if is_LOGGING
  sdLoggger.logData(); // Log Data to SD Card when not on IDLE or LANDED state
#endif
}

  //delay(BNO055_SAMPLERATE_DELAY_MS);


























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
