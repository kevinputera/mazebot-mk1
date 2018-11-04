#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define BUTTON A7
// define constants for P controller
#define KP 4
#define SETPOINT 935
#define BASE_SPEED 255

// initialize left and right motor objects
MeDCMotor motor_left(M2);
MeDCMotor motor_right(M1);
// initialize line detector object
MeLineFollower line_detector(PORT_1);

void setup() {
    Serial.begin(9600);
  
    // initialize left and right IR sensor pins
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);

    // initialize start button
    // press then release button to begin the run
    pinMode(BUTTON, INPUT);
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}
}

void loop() {
    // run wall following so long as no black line is detected
    // if (line_detector.readSensor1 == 1) {
    int state = get_IR_dist(IR_LEFT);
    // Serial.println(state);
    p_controller_run(KP, state, SETPOINT, BASE_SPEED);
    // }
}

// function for the PID controller
void p_controller_run(float kp, int state, int setpoint, int base_speed) {
    float error = state - setpoint;
    float output = (kp * error);

    // adjust output to account for the non-linear nature of the IR reading
    if (output > 0) {
        output *= 2;
    }

    // convert output into change in motor speeds
    int speed_left = (base_speed - output) * (-1);
    int speed_right = (base_speed + output);
    motor_left.run(speed_left);
    motor_right.run(speed_right);
}

// function to get the distance of the vehicle from the wall
// we'll use the value 0-1023 to represent the distance
int get_IR_dist(int pin) {
    int dist = analogRead(pin);
    return dist;
}
