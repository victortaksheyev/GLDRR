#ifndef IMU_H
#define IMU_H

class IMU {
    public:
        IMU();
        bool begin();
        void sample();
        bool detectLiftoff();
    private:
        bool cal;
        uint8_t sysCal, gyroCal, accelCal, magCal;
        int liftoffDetectionMeasures;
};

#endif /* IMU_H */
