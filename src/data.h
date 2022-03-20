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
    ABORT = 6,
    TEST = 7,
};

void goToState(State_t state);

struct Telemetry_t
{
    Telemetry_t() {state = 0;}
    State_t state;    // State of the system
    float delta_t;    // Discrete Time
    float flightTime; // Keeps track of the time after liftoff
    float sampleRate; // samples per second
    axis_t accel;
    axis_t grav;
    axis_t angV;
    float altitude;
    float prevAltitude;
    float verticalVelocity;
    float maxAltitude;
    // TODO: add GPS
};

extern Telemetry_t data;

#endif /* DATA_H */
