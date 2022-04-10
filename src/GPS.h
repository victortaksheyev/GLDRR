#ifndef GPS_H
#define GPS_H

class GPS {
    public:
        bool begin();
        void sample();
        float calcBearing();

    private:
        void clearGPS();
        char c;
};

#endif /* GPS_H */
