#include "Adafruit_BMP3XX.h"
#include "Altimeter.h"
#include "MAF.h"
#include "data.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define BMP_SCK 13
#define BMP_CS 10

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
    for (int i = 0; i < 50; i++)
    {
      seaLevelOffset += bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
    this->seaLevelOffset /= 50;
    this->altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - this->seaLevelOffset;
    this->prevAltitude = 0; // make sure this is 0
    return true;
  }
  else{
    return false;
  }
}

void Altimeter::sample() {
    float alt;
    if (bmp.performReading()){
        alt = bmp.readAltitude(SEALEVELPRESSURE_HPA) - this->seaLevelOffset;
    }

    this->altitude = MAF_altitude.process(alt);
    this->verticalVelocity = MAF_velocity.process(getVerticalVelocity());

    // Record the max altitude
    if (this->altitude > this->maxAltitude){
        this->maxAltitude = this->altitude;
    }

    // Replace the previous_altitude with the current altitude for next loop
    this->prevAltitude = this->altitude;
    return true;
}

float Altimeter::getVerticalVelocity(){
  return (this->altitude - this->prevAltitude) / data.delta_t;
}

bool Altimeter::detectApogee(){
  if ((this->altitude < this->maxAltitude)){
    // if (apogeeTimer.hasPassed(100)){
    //   Serial.printf("\n---- APOGGE! ---- Max altitude: %f", max_altitude);
    //   return true;
    // }
    return true;
  }
  return false;
}

float Altimeter::getAltitude() {
    return this->altitude;
}
