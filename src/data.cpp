#include "data.h"
#include <math.h>

Telemetry_t data;
void goToState(State_t state) {
    data.state = state;
}

float accelMag(float x, float y, float z) {
  return sqrt(x*x + y*y + z*z);
}

Telemetry_t::Telemetry_t() {
  this->state = CHECK;
  this->delta_t = 0;
  this->flightTime = 0;
  this->accel.x = this->accel.y = this->accel.z = 0;
  this->angV.x = this->angV.y = this->angV.z = 0;
  this->altitude = this->prevAltitude = 0;
  this->verticalVelocity = 0; 
  this->maxAltitude = 0;
  this->GPSfix = false;
  this->heading = 0;
  this->latCurr = this->lonCurr = 0;
  this->error = 0;
}
