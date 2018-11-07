#include <MeMCore.h>

#define BUTTON A7

MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);

void setup() {
    pinMode(BUTTON, INPUT);
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}

    // turn right
    motor_left.run(-255);
    motor_right.run(-255);
    delay(290);
    motor_left.stop();
    motor_right.stop();
    delay(1000);

    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}

    // turn left
    motor_left.run(255);
    motor_right.run(255);
    delay(290);
    motor_left.stop();
    motor_right.stop();

    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}

    // turn 180
    motor_left.run(255);
    motor_right.run(255);
    delay(550);
    motor_left.stop();
    motor_right.stop();
}

void loop() {

}
