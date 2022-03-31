#include "logger.h"
#include "config.h"

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

bool Logger::writeString(String data) {
  this->file = SD.open(this->filename, FILE_WRITE);
  if (this->file) {
    this->file.println(data);
    this->file.close();
  } else {
     return false;
  }  
  return true;
}

void Logger::close() {
    this->file.close();
}
