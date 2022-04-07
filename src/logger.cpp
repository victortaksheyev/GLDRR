#include "logger.h"
#include "config.h"
#include "data.h"

Logger::Logger(String filename) {
    pinMode(CHIP_SELECT, OUTPUT);

    // initialize file
    this->filename = filename;
}

bool Logger::begin() {
    if (!SD.begin(CHIP_SELECT))
      return false;
    return true;
}

bool Logger::writeData() {
  this->file = SD.open(this->filename, FILE_WRITE);
  if (this->file) {
    this->file.print(data.flightTime);this->file.print("|");this->file.print(data.state);this->file.print("|");
    this->file.print(data.accel.z);this->file.print("|");this->file.print(accelMag(data.accel.x, data.accel.y, data.accel.z));this->file.print("|");
    this->file.print(data.angV.x);this->file.print("|");this->file.print(data.angV.y);this->file.print("|");this->file.print(data.angV.z);this->file.print("|");
    this->file.print(data.altitude);this->file.println("|");
    this->file.close();

    // update heading, distance, latitude, longitude, servo commands
  } else {
     return false;
  }  
  return true;
}

void Logger::close() {
    this->file.close();
}
