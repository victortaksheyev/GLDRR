
// CLK - 13
// DO (MISO) - 12
// DI (MOSI) - 11
// CS - 10

#ifndef LOGGER_H
#define LOGGER_H

#include <SD.h>
#include <string.h>

class Logger {
    public:
        Logger(String filename);
        void close();
        bool writeString(String);
        bool begin();
    private:
        File file;
        String filename;
};

#endif /* LOGGER_H */
