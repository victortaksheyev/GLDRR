#include "logger.h"
#include "config.h"
#include "data.h"

bool Logger::begin() {
    pinMode(CHIP_SELECT, OUTPUT);
    digitalWrite(CHIP_SELECT, HIGH);
    if (!SD.begin(CHIP_SELECT))
      return false;
    return true;
}

bool Logger::writeData() {
  File dataFile = SD.open(FILENAME, FILE_WRITE);
  if (dataFile) {
    dataFile.print(data.flightTime);dataFile.print(F("|"));dataFile.print(data.state);dataFile.print(F("|"));
    dataFile.print(data.accel.z);dataFile.print(F("|"));dataFile.print(accelMag(data.accel.x, data.accel.y, data.accel.z));dataFile.print(F("|"));
    dataFile.print(data.angV.x);dataFile.print(F("|"));dataFile.print(data.altitude);dataFile.print(F("|"));dataFile.print(degrees(data.latCurr),7);dataFile.print(F("|"));dataFile.print(degrees(data.lonCurr),7);dataFile.print(F("|"));
    dataFile.print(data.heading);dataFile.print(F("|"));dataFile.print(data.error);dataFile.print(F("|"));dataFile.println(data.servoCommandF, 2);
    dataFile.close();
  } else {
     return false;
  }  
  return true;
}
