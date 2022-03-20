// better idea: have a sensor class (abstract) that imu, altimeter, GPS derive from
#ifndef IMU_H
#define IMU_H

struct axis_t
{
    float x;
    float y;
    float z;
};

class IMU {
    public:
        bool begin();
        bool calibrated();
        void sample();
        void print();
        axis_t getAccel() {return accel;}
        axis_t getGrav() {return grav;}
        axis_t getAngv() {return angV;}

    // private:
        axis_t accel;
        axis_t grav;
        axis_t angV;
};

#endif /* IMU_H */