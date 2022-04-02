
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "GPS.h"
#include "data.h"

// you can change the pin numbers to match your wiring:
SoftwareSerial mySerial(TX, RX);
Adafruit_GPS GPSSerial(&mySerial);

GPS::GPS(){
    firstSample = true;
}

bool GPS::begin() {
    GPSSerial.begin(9600);
    GPSSerial.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPSSerial.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    delay(1000);
}

void GPS::sample() {
    if (GPSSerial.newNMEAreceived()) {
    if (!GPSSerial.parse(GPSSerial.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
    }
    if (GPSSerial.fix) {
        data.GPSfix = true;
        // get lat degrees
        data.latCurr = radians(GPSSerial.latitudeDegrees);
        data.lonCurr = radians(GPSSerial.longitudeDegrees);
        if (firstSample) {
            firstSample = false;
            data.latInit = radians(GPSSerial.latitudeDegrees);
            data.lonInit = radians(GPSSerial.longitudeDegrees);
        }
    } else {
        data.GPSfix = false;
        // use previous data as the current;
    }
}

float GPS::calcBearing() {
   //==================Heading Formula Calculation================//
   float y = sin(data.latCurr-data.latInit) * cos(data.latCurr);
   float x = cos(data.latInit)*sin(data.latCurr) - sin(data.latInit)*cos(data.latCurr)*cos(data.latCurr-data.latInit);
   float brng = atan2(y,x);
   brng = degrees(brng);// radians to degrees
   brng = ( ((int)brng + 360) % 360 ); 
   data.heading = brng;
}

 float GPS::calcDistance() {
   float a = sin(data.latCurr - data.latInit / 2) * sin(data.latCurr - data.latInit / 2) + cos(data.latInit) * cos(data.latCurr) * sin(data.lonCurr - data.lonInit / 2) * sin (data.lonCurr - data.lonInit/ 2);
   float c = 2 * atan2(sqrt(a), sqrt(1-a));
   float d = EARTH_RAD * c;
   return d;
  }
