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
 
        bool firstSample;
};

#endif /* GPS_H */
