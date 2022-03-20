#include "Adafruit_BMP3XX.h"
#include "Altimeter.h"
#include "MAF.h"
#include "data.h"
#include "config.h"

Adafruit_BMP3XX bmp;
MovingAverageFilter MAF_altitude(10);
MovingAverageFilter MAF_velocity(20);

bool Altimeter::begin()
{
  // Start sensor on I2C
  if (bmp.begin_I2C()){

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_200_HZ);

    for (int i = 0; i < 50; i++) // let the sensor make a few readings to get it started
    {
      bmp.readPressure();
      bmp.readTemperature();
    }

    // Just take the average for (relative Altitude determination)
    for (int i = 0; i < SEA_LEVEL_DETERMINATION_INSTANCES; i++)
    {
      seaLevelOffset += bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
    this->seaLevelOffset /= SEA_LEVEL_DETERMINATION_INSTANCES;
    data.altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - this->seaLevelOffset;
    data.prevAltitude = data.altitude;
    return true;
  }
  else{
    return false;
  }
}

void Altimeter::sample() {
    float alt;
    if (bmp.performReading()){
        alt = bmp.readAltitude(SEALEVELPRESSURE_HPA) - getSeaLevelOffset();
    }

    data.altitude = MAF_altitude.process(alt);
    data.verticalVelocity = MAF_velocity.process(getVerticalVelocity());

    // Record the max altitude
    if (data.altitude > data.maxAltitude){
        data.maxAltitude = data.altitude;
    }

    // Replace the previous_altitude with the current altitude for next loop
    data.prevAltitude = data.altitude;
    return true;
}

float Altimeter::getVerticalVelocity(){
  return (data.altitude - data.prevAltitude) / data.delta_t;
}

bool Altimeter::detectApogee(){
  if (data.altitude < data.maxAltitude){
    // ensure multiple instances of repeated altitude decrease
    this->apogeeDetectionMeasures -= 1;
    if (this->apogeeDetectionMeasures == 0) return true;
    else this->apogeeDetectionMeasures = ALTIMERER_APOGEE_DETECTION_MEASURES;
  }
  return false;
}

bool Altimeter::detectLiftoff() {
  // for # of measures, above liftoff threshold and increase of 3m each measure
  if (data.altitude > LIFTOFF_ALTITUDE_THRESHOLD &&
      (data.altitude - data.prevAltitude) > LIFTOFF_DELTA_THRESHOLD) {
    this->liftoffDetectionMeasures -= 1;
    if (this->liftoffDetectionMeasures == 0) return true;
    else this->liftoffDetectionMeasures = ALTIMETER_LIFTOFF_DETECTION_MEASURES;
  }
  return false;
}

float Altimeter::getSeaLevelOffset() {
  return this->seaLevelOffset;
}

Altimeter::Altimeter() {
    this->seaLevelOffset = 0;
    this->apogeeDetectionMeasures = ALTIMERER_APOGEE_DETECTION_MEASURES;
    this->liftoffDetectionMeasures = ALTIMETER_LIFTOFF_DETECTION_MEASURES;
}