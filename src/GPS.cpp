

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "GPS.h"
#include "data.h"

// you can change the pin numbers to match your wiring:
SoftwareSerial mySerial(TX, RX);
Adafruit_GPS GPSSerial(&mySerial);

GPS::GPS(){
//    firstSample = true;
  firstSample = false;
  data.latInit = radians(36.058966);
  data.lonInit = radians(-115.221528);
}

// TODO: change to void
bool GPS::begin() {
  GPSSerial.begin(9600);
  GPSSerial.sendCommand("$PGCMD,33,0*6D");
  GPSSerial.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  GPSSerial.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
 
  return true;
}

void GPS::clearGPS() {
  for (int i = 0; i < 2; i++) {
    while(!GPSSerial.newNMEAreceived())
      c = GPSSerial.read();  
    GPSSerial.parse(GPSSerial.lastNMEA());
  }
}

void GPS::sample() {
    clearGPS();
    while(!GPSSerial.newNMEAreceived()) {
      c = GPSSerial.read();  
    }
    GPSSerial.parse(GPSSerial.lastNMEA());

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
   float y = sin(data.lonInit-data.lonCurr) * cos(data.latInit);
   float x = cos(data.latCurr)*sin(data.latInit) - sin(data.latCurr)*cos(data.latInit)*cos(data.lonInit-data.lonCurr);
   float brng = atan2(y,x);
   brng = degrees(brng);// radians to degrees
   brng = ( ((int)brng + 360) % 360 ); 
   data.heading = brng;
}

 float GPS::calcDistance() {
   float a = sin((data.latInit - data.latCurr) / 2) * sin((data.latInit - data.latCurr) / 2) + cos(data.latCurr) * cos(data.latInit) * sin((data.lonInit - data.lonCurr) / 2) * sin ((data.lonInit - data.lonCurr)/ 2);
   float c = 2 * atan2(sqrt(a), sqrt(1-a));
   float d = EARTH_RAD * c;
   return d;  // meters
  }
