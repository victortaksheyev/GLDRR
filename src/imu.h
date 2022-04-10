// better idea: have a sensor class (abstract) that imu, altimeter, GPS derive from
#ifndef IMU_H
#define IMU_H

class IMU {
    public:
        IMU();
        bool begin();
        void calibrate();
        bool calibrated();
        void sample();
        bool detectLiftoff();
    private:
        bool cal;
        uint8_t sysCal, gyroCal, accelCal, magCal;
        int liftoffDetectionMeasures;
};

#endif /* IMU_H */
