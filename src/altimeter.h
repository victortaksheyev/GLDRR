#ifndef ALTIMETER_H
#define ALTIMETER_H



class Altimeter {
    public:
        Altimeter() {
            altitude = prevAltitude = verticalVelocity = seaLevelOffset = maxAltitude = 0;
        }
        bool begin();
        void print();
        void sample();
        bool detectApogee();
        float getVerticalVelocity();
        float getAltitude();
    // private:
        float altitude;
        float prevAltitude;
        float verticalVelocity;
        float seaLevelOffset;
        float maxAltitude;
};

#endif /* ALTIMETER_H */
