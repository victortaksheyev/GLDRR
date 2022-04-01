// main setup and loop for GLDRR
// for IMU (BNO055), reference read_all_data example
// for Altimeter (BMP390), reference simple_test
#include <Wire.h>
#include <Arduino.h>
#include "data.h"
#include "imu.h"
#include "altimeter.h"
//#include "logger.h"

Altimeter altimeter;
IMU imu;
//Logger logger("GLDRRlog.txt");
//(TODO) GPS
//(TODO) servo

unsigned long prevLoopTime, currentLoopTime, startTime;

float bearing(float lat,float lon,float lat2,float lon2){

    float teta1 = radians(lat);
    float teta2 = radians(lat2);
    float delta1 = radians(lat2-lat);
    float delta2 = radians(lon2-lon);

    //==================Heading Formula Calculation================//

    float y = sin(delta2) * cos(teta2);
    float x = cos(teta1)*sin(teta2) - sin(teta1)*cos(teta2)*cos(delta2);
    float brng = atan2(y,x);
    brng = degrees(brng);// radians to degrees
    brng = ( ((int)brng + 360) % 360 ); 

    Serial.print("Heading GPS: ");
    Serial.println(brng);

    return brng;


  }

  float distance(float lat1, float lon1, float lat2, float lat3) {
    float R = 6371000;
    float theta_1 = lat1 * PI / 180;
    float theta_2 = lat2 * PI/ 180;
    float delta_theta = (lat2 - lat1) * PI/180;
    float delta_L = (lon2-lon1) * PI/180;

    float a = sin(delta_theta / 2) * sin(delta_theta / 2) + cos(theta_1) * cos(theta_2) * sin(delta_L / 2) * sin (delta_L / 2);
    float c = 2 * atan2(sqrt(a, sqrt(1-a));
    float d = R * c;
   }



void setup() {
  Serial.begin(115200);

  // IMU - BNO055
  if (!imu.begin())
  {
    Serial.print(F("imu begin failed"));
    while (1);
  }
  // Altimeter - BMP390
  if (!altimeter.begin()) {
    Serial.print(F("altimeter begin failed"));
    while(1);
  }
  // Logger
//  if (!logger.begin()) {
//    Serial.print(F("logger begin failed"));  
//  }
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
  // ensure IMU is calibrated
  /*
  while (!imu.calibrated()) {
    imu.calibrate();
    Serial.println(F("IMU calibrating"));
  }
  */

  // ensure GPS has a fix

  // sensor timing
  currentLoopTime = micros();
  data.delta_t = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;


  String dataString = "";
  dataString += String(data.flightTime) + "|" + String(data.state) + "|";
  dataString += String(data.accel.z) + "|" + String(accelMag(data.accel.x, data.accel.y, data.accel.z)) + "|";
  dataString += String(data.angV.x) + "|" + String(data.angV.y) + "|" + String(data.angV.z) + "|";
  dataString += String(data.altitude) + "|" + String(data.verticalVelocity);
  // distance to location
  // desired vs actual heading
  // update dataDoc
  // (θ+360) % 360 to convert -180 -> 180 into 0 -> 360.


  /*
   * Distance calcualtion:
   * const R = 6371e3; // metres
    const φ1 = lat1 * Math.PI/180; // φ, λ in radians
    const φ2 = lat2 * Math.PI/180;
    const Δφ = (lat2-lat1) * Math.PI/180;
    const Δλ = (lon2-lon1) * Math.PI/180;
    
    const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
              Math.cos(φ1) * Math.cos(φ2) *
              Math.sin(Δλ/2) * Math.sin(Δλ/2);
    const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
    
    const d = R * c; // in meters
   */

   /*
    * 
    *   const x = (λ2-λ1) * Math.cos((φ1+φ2)/2);
        const y = (φ2-φ1);
        const d = Math.sqrt(x*x + y*y) * R;
    */

  Serial.println(dataString);
  
  // flight timestamp
  if (data.state <= PAD){
    data.flightTime = 0.0;
    startTime = millis();
  } else if (data.state > PAD && data.state < LANDED) {
    data.flightTime = float((millis() - startTime) / 1000.0f);
    // log data
  } else {
    // maintain current time
    data.flightTime = data.flightTime;
  }
 
  // functions to run every loop
  imu.sample();
  altimeter.sample();

  // log data
//  bool res = logger.writeString("test data");
 
  // state machine
  switch (data.state){
    case CHECK:
//      Serial.println(F("IDLE"));
      goToState(PAD);
      break;

    case PAD:
        goToState(ASCENT);
//      Serial.println(F("PAD"));
      if (imu.detectLiftoff() || altimeter.detectLiftoff())
        goToState(ASCENT);
      break;

    case ASCENT:
      if (altimeter.detectApogee())
        goToState(APOGEE);
      break;
    
    case APOGEE:
      goToState(DESCENT);
      break;
    
    case DESCENT:
      break;
    
    case LANDED:
      break;
    
    case ABORT:
      break;
    
    case TEST:
      break;
  }

}
