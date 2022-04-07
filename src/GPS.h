#ifndef GPS_H
#define GPS_H

class GPS {
    public:
        GPS();
        bool begin();
        void sample();
        float calcBearing();
        float calcDistance();
    private:
        void clearGPS();
        bool firstSample;
        char c;
};

#endif /* GPS_H */
