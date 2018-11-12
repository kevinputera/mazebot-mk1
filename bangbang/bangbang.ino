#include <MeMCore.h>

#define IR_LEFT A0
#define IR_RIGHT A1
#define BUTTON A7

#define SETPOINT_LEFT 660
#define SETPOINT_RIGHT 630
#define BASE_SPEED 255

MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);

void setup() {
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
    pinMode(BUTTON, INPUT);
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}
}

void loop() {
    int state_left = get_IR_reading(IR_LEFT);
    int state_right = get_IR_reading(IR_RIGHT);

    if (state_left < (SETPOINT_LEFT)) {
        motor_left.run(-255);
        motor_right.run(50);
    } else if (state_right < (SETPOINT_RIGHT)) {
        motor_left.run(-50);
        motor_right.run(255);
    } else {
        motor_left.run(-255);
        motor_right.run(240);
    }
}

int get_IR_reading(int pin) {
    int raw = analogRead(pin);
    return raw;
}
