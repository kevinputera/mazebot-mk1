#include <MeMCore.h>

#define BUTTON A7

MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);

void setup() {
    pinMode(BUTTON, INPUT);
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}
}

void loop() {
  motor_left.run(-255);
  motor_right.run(255);
}
