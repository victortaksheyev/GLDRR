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
    posCommand = static_cast<int>(Kp*error + Ki*(cumError) + Kd*((error - prevError)/elapsedTime));
   
    
    if (posCommand > 500)
        posCommand = 500;
    else if (posCommand < -500)
        posCommand = -500;
    posCommand = map(posCommand, -500, 500, -90, 90);
    data.servoCommand = posCommand + 90;
    // degrees
    if (abs(error) < 10) {
        // acceptable, don't command a change
    } else {
        90 + posCommand;
        // servo.write(command);
    }

    prevError = error;                                //remember current error
    prevTime = currTime;                        //remember current time
}
