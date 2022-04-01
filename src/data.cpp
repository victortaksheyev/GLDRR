#include "data.h"
#include <math.h>

Telemetry_t data;
void goToState(State_t state) {
    data.state = state;
}

float accelMag(float x, float y, float z) {
  return sqrt(x*x + y*y + z*z);
}

//char* createDataString() {
//  char* dataString = malloc(sizeof(char) * 20);
//  dataString += String(data.state) + "|";
//  dataString += String(data.delta_t) + "|";
////  data.flightTime
////  data.accel.x
////  data.accel.y
////  data.accel.z
////
////  data.grav.x
////  data.grav.y
////  data.grav.z
////
////  data.angV.x
////  data.angV.y
////  data.angV.z
////
////  data.altitude
////  data.prevAltitude
////  data.verticalVelocity
////  data.maxAltitude
//
//  return dataString;
//}


/*  state_t state;    // State of the system
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
 */
