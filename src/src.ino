// main setup and loop for GLDRR
// for IMU (BNO055), reference read_all_data example
// for Altimeter (BMP390), reference simple_test
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "Adafruit_BMP3XX.h"

/* begin - initialize IMU */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
uint8_t sysCal, gyroCal, accelCal, magCal = 0; // calibration flags
/* end - initialize IMU */

/* begin - initialize altimeter */
#define BMP_SCK 13
#define BMP_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp;
float alt = 0;
/* end - initialize altimeter */

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(115200);
  // IMU - BNO055
  if (!bno.begin())
  {
    Serial.print("no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  // Altimeter - BMP390
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
  /* begin - aquire IMU (BNO055) data */ 
  sensors_event_t angVelocityData, accelerometerData, gravityData;
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
 if (imuCalibrated()) {
    printEvent(&angVelocityData);
    printEvent(&accelerometerData);
    printEvent(&gravityData);
  } else {
   printIMUCalibration();
  }
  /* end - aquire IMU (BNO055) data */ 

  /* begin - quire Altimeter (BMP390) data */
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("alt: ");
  Serial.println(alt);
  /* end - quire Altimeter (BMP390) data */

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

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

/* prints altimeter data */


/* Returns true if IMU (BNO055) is calibrated - false otherwise */
bool imuCalibrated() {
  bno.getCalibration(&sysCal, &gyroCal, &accelCal, &magCal);
  if (gyroCal == 3 && accelCal == 3 && magCal == 3)
    return true;
   return false;
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
