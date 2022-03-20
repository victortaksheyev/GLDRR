#ifndef CONFIG_H
#define CONFIG_H

// constants
#define EARTH_ACCEL (9.8067f)
#define SEALEVELPRESSURE_HPA (1013.25)

// serial debugging
#define DEBUG false
#define SERIAL_BAUD (115200)

// thresholds
#define LIFTOFF_ALTITUDE_THRESHOLD (2.0f) // m
#define LIFTOFF_DELTA_THRESHOLD (3.0f)    // m
#define LAUNCH_ACCEL_THRESHOLD (3.0f)     // gs


#define ACCEL_UNPOWERED_THRESHOLD (1.5f)  // m/s^2


#define ALTIMERER_APOGEE_DETECTION_MEASURES (10)
#define ALTIMETER_LIFTOFF_DETECTION_MEASURES (5)
#define IMU_LIFTOFF_DETECTION_MEASURES (10)
#define SEA_LEVEL_DETERMINATION_INSTANCES (50)

// GPS

// LED

// PINS
    // sd card stuff


#endif /* CONFIG_H */