#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "imu.h"
#include "data.h"
#include "config.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

IMU::IMU() {
    sysCal = gyroCal = accelCal = magCal = 0;
    liftoffDetectionMeasures = IMU_LIFTOFF_DETECTION_MEASURES;
}

bool IMU::begin() {
    if (!bno.begin()) return false;
    return true;
}

bool IMU::calibrated() {
  if(this->cal) return true;
  return false;  
}

void IMU::calibrate() {
    bno.getCalibration(&sysCal, &gyroCal, &accelCal, &magCal);
    if (gyroCal == 3 && accelCal == 3 && magCal == 3) this->cal = true;
}

bool IMU::detectLiftoff() {
    if ((data.accel.x - EARTH_ACCEL) >= (LAUNCH_ACCEL_THRESHOLD * EARTH_ACCEL)) {
        this->liftoffDetectionMeasures -= 1;
        if (this->liftoffDetectionMeasures == 0) return true;
        else this->liftoffDetectionMeasures = IMU_LIFTOFF_DETECTION_MEASURES;
    }
    return false;
}

void IMU::sample() {
    // TODO: get rid of additional sensors_event_t variable
    sensors_event_t angVelocityData, accelerometerData, gravityData;
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

    data.angV.x = angVelocityData.gyro.x;
    data.angV.y = angVelocityData.gyro.y;
    data.angV.z = angVelocityData.gyro.z;

    data.accel.x = accelerometerData.acceleration.x;
    data.accel.y = accelerometerData.acceleration.y;
    data.accel.z = accelerometerData.acceleration.z;

    data.grav.x = gravityData.acceleration.x;
    data.grav.y = gravityData.acceleration.y;
    data.grav.z = gravityData.acceleration.z;
}

void IMU::print() {
    //? can we use serial here?
}