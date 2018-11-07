#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define BUTTON A7
// define constants for P controller
#define KP_LEFT 1
#define KP_RIGHT 3.5
#define SETPOINT_LEFT 725
#define SETPOINT_RIGHT 360
#define BASE_SPEED 255

// initialize left and right motor objects
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
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
    /*Serial.print("LEFT: ");
    Serial.println(get_IR_dist(IR_LEFT));
    Serial.print("RIGHT: ");
    Serial.println(get_IR_dist(IR_RIGHT));*/
    if (line_detector.readSensor1() == 0) {
        motor_left.stop();
        motor_right.stop();
    } else {
        int state_left = get_IR_dist(IR_LEFT);
        int state_right = get_IR_dist(IR_RIGHT);
        if (state_right > 430) {
            motor_left.run(-255);
            motor_right.run(255);
        }
        Serial.println(state_left);
        Serial.println(state_right);
        //p_controller_run(KP_LEFT, KP_RIGHT, state_left, state_right, SETPOINT_LEFT, SETPOINT_RIGHT, BASE_SPEED);
    }
}

// function for the PID controller
void p_controller_run(float kp_left, float kp_right, int state_left, int state_right, int setpoint_left, int setpoint_right, int base_speed) {
    float error_left = state_left - setpoint_left;
    float output_left = kp_left * error_left;
    float error_right = state_right - setpoint_right;
    float output_right = kp_right * error_right;

    if (output_right > 0) {
        output_right *= 1.2;
    }

    // convert output into change in motor speeds
    int speed_left = (base_speed + output_right) * (-1);
    int speed_right = (base_speed - output_right);
    
    /*Serial.print("LEFT: ");
    Serial.println(speed_left);
    Serial.println("RIGHT: ");
    Serial.println(speed_right);*/
    motor_left.run(speed_left);
    motor_right.run(speed_right);
}

// function to get the distance of the vehicle from the wall
// we'll use the value 0-1023 to represent the distance
int get_IR_dist(int pin) {
    int dist = analogRead(pin);
    return dist;
}
