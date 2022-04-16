#ifndef ALTIMETER_H
#define ALTIMETER_H

class Altimeter {
    public:
        Altimeter();
        bool begin();
        void print();
        void sample();
        bool detectApogee();
        bool detectLiftoff();
        bool detectLanding();
        float getSeaLevelOffset();
        float getVerticalVelocity();
    private:
        float seaLevelOffset;
        int apogeeDetectionMeasures;   // number of measures to confirm apogee detection
        int liftoffDetectionMeasures;
        int landingDetectionMesasures;
};

#endif /* ALTIMETER_H */
