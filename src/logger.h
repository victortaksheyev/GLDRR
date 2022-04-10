
// CLK - 13
// DO (MISO) - 12
// DI (MOSI) - 11
// CS - 10

#ifndef LOGGER_H
#define LOGGER_H

#include <SD.h>
#include <SPI.h>

class Logger {
    public:
        bool writeData();
        bool begin();
};

#endif /* LOGGER_H */
