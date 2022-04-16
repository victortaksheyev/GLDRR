#ifndef CONFIG_H
#define CONFIG_H

#define FILENAME ("test2.txt")

// constants
#define EARTH_ACCEL (9.8067f)
#define SEALEVELPRESSURE_HPA (1013.25)
#define EARTH_RAD (6371000)

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
#define TX (0)
#define RX (1)
#define GPSECHO  false

// LED

// PINS
#define CHIP_SELECT (10)
#define SERVO2 (4)
#define SERVO1 (3)
#define REG_ENABLE (2)
#define BUZZER (9)
#define CAM1 (5)
#define CAM2 (6)

// PID
#define Kp (3)
#define Ki (0.02)
#define Kd (3)



#endif /* CONFIG_H */
