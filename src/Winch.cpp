#include <Servo.h>
#include <Arduino.h>
#include "config.h"
#include "Winch.h"
#include "data.h"

Servo servo;

Winch::Winch() {
    pos = 90; // 0 position
}

void Winch::begin() {
    servo.attach(SERVO2);
    // ensure initial position
    servo.write(pos);
    delay(3000);

    // visual confirmation
    servo.write(pos + 3);
    delay(300);
    servo.write(pos - 3);
    delay(600);
    servo.write(pos);
    delay(1000);
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void Winch::command() {
    unsigned long currTime = millis();
    float elapsedTime = (float)(currTime - prevTime);

    // desired - current
    float error = data.heading - data.angV.x;
    if (error < -180)
        error += 360;
    else if (error > 180)
        error -= 360;

    data.error = error;
    cumError += error * elapsedTime/1000;
    // compute the value to command tot he servo
    posCommand = Kp*error + Ki*cumError + Kd*((error - prevError)/elapsedTime);
    
    if (posCommand > 500)
        posCommand = 500;
    else if (posCommand < -500)
        posCommand = -500;
    
    data.servoCommandF = 90 + mapfloat(posCommand, -500, 500, -15, 15);
    data.servoCommand = (int)data.servoCommandF;
    // degrees
    if (abs(error) < 15) {
        // acceptable, don't command a change
    } else {
         servo.write(data.servoCommand);
    }

    prevError = error;                                //remember current error
    prevTime = currTime;                        //remember current time
}
