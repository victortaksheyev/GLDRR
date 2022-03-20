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
        float getSeaLevelOffset();
    private:
        float seaLevelOffset;
        int apogeeDetectionMeasures;   // number of measures to confirm apogee detection
        int liftoffDetectionMeasures;
};

#endif /* ALTIMETER_H */
