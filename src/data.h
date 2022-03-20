#ifndef DATA_H
#define DATA_H


// #include <vector.h>

/*
This data library is inspired on Adam Marciniak! (his way of organizing everything in one struct and use it throught the software)
Go show him some love https://github.com/AdamMarciniak
*/

// extern uint16_t rateHz;
// extern uint16_t numSeconds;
// extern uint16_t totalSamples;
// extern uint16_t millisPerSample;

enum State_t
{
    IDLE = 0,    // LAUNCHPAD
    LIFTOFF = 1, // ARMED waiting liftoff
    POWERED_ASCENT = 2,
    MECU = 3, // MECU
    APOGGE = 4,
    PARACHUTE_DESCENT = 5,
    LANDED = 6,
    ABORT = 7,
    TEST = 8, // run specific code (debug mode rigth?)
};
void goToState(State_t state);
struct Telemetry_t
{
    State_t state;    // State of the system
    float delta_t;    // Discrete Time
    float flightTime; // Keeps track of the time after liftoff
    float sampleRate; // samples per second
};

extern Telemetry_t data;

#endif /* DATA_H */
