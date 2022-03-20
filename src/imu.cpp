#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "imu.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
uint8_t sysCal, gyroCal, accelCal, magCal = 0; // calibration flags

bool IMU::begin() {
    if (!bno.begin()) return false;
    return true;
}

bool IMU::calibrated() {
    bno.getCalibration(&sysCal, &gyroCal, &accelCal, &magCal);
    if (gyroCal == 3 && accelCal == 3 && magCal == 3) return true;
    return false;
}

void IMU::sample() {
    // TODO: get rid of additional sensors_event_t variable
    sensors_event_t angVelocityData, accelerometerData, gravityData;
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

    this->angV.x = angVelocityData.gyro.x;
    this->angV.y = angVelocityData.gyro.y;
    this->angV.z = angVelocityData.gyro.z;

    this->accel.x = accelerometerData.acceleration.x;
    this->accel.y = accelerometerData.acceleration.y;
    this->accel.z = accelerometerData.acceleration.z;

    this->grav.x = gravityData.acceleration.x;
    this->grav.y = gravityData.acceleration.y;
    this->grav.z = gravityData.acceleration.z;
}

void IMU::print() {
    //? can we use serial here?
}
