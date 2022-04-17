#ifndef DATA_H
#define DATA_H

struct axis_t
{
    float x;
    float y;
    float z;
};

enum State_t
{
    CHECK = 0, 
    PAD = 1,
    ASCENT = 2,
    APOGEE = 3,
    DESCENT = 4,    // control algorithm runs
    LANDED = 5,
    TEST = 6,
};

void goToState(State_t state);
float accelMag(float x, float y, float z);

struct Telemetry_t
{
    Telemetry_t();
    State_t state;    // State of the system
    float delta_t;    // Discrete Time
    float flightTime; // Keeps track of the time after liftoff
    axis_t accel;
    axis_t angV;
    float altitude;
    float prevAltitude;
    float verticalVelocity;
    float maxAltitude;
    bool GPSfix;   
    float heading; // deg
    float distance;
    float latCurr, latInit, lonCurr, lonInit;
    int servoCommand;
    float servoCommandF;
    float error;
};

extern Telemetry_t data;

#endif /* DATA_H */
