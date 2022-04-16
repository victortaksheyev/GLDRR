#include "config.h"

void camOff() {
    digitalWrite(CAM1, LOW);
    digitalWrite(CAM2, LOW);
}

void camInit() {
    pinMode(CAM1, OUTPUT);
    pinMode(CAM2, OUTPUT);
    camOff();
}

void camOn() {
    digitalWrite(CAM1, HIGH);
    digitalWrite(CAM2, HIGH);
}

void buzzError() {
    // deep, error sound
    for (int i = 0; i < 3; i++) {
        tone(BUZZER, 200);
        delay(500);
        noTone(BUZZER);
        delay(100);
    }
}

void buzzSuccess() {
    // pulsing for flight
    for (int i = 0; i < 3; i++) {
      tone(BUZZER, 1000);
      delay(200);
      tone(BUZZER, 2000);
      delay(500);
    }
   
    noTone(BUZZER);
}

void buzzGPS() {
    // short buzz
    tone(BUZZER, 1000);
    delay(20);
    noTone(BUZZER);
}
