#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define BUTTON A7
// define constants for P controller
#define KP_LEFT 2.2
#define KP_RIGHT 2.2
#define KI_LEFT 0.1
#define KI_RIGHT 0.1
#define SETPOINT_LEFT 546
#define SETPOINT_RIGHT 717
#define BASE_SPEED 255

// initialize left and right motor objects
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
// initialize line detector object
MeLineFollower line_detector(PORT_1);

float integral_left = 0;
float integral_right = 0;

int count = 1;

void setup() {
    Serial.begin(9600);
  
    // initialize left and right IR sensor pins
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);

    pinMode(13, INPUT);

    // initialize start button
    // press then release button to begin the run
    pinMode(BUTTON, INPUT);
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}
}

void loop() {
    // run wall following so long as no black line is detected
    if (line_detector.readSensor1() == 0) {
        motor_left.stop();
        motor_right.stop();
        delay(200);

        if (count == 1 || count == 2) {
            // turn right
            motor_left.run(-255);
            motor_right.run(-255);
            delay(290);
            motor_left.stop();
            motor_right.stop();

        } else if (count == 3) {
            // turn left
            motor_left.run(255);
            motor_right.run(255);
            delay(290);
            motor_left.stop();
            motor_right.stop();
          
        } else if (count == 4) {
            // turn 180
            motor_left.run(255);
            motor_right.run(255);
            delay(550);
            motor_left.stop();
            motor_right.stop();
          
        } else if (count == 5) {
            motor_left.stop();
            motor_right.stop();
            while (true) {}
        }

        count += 1;
        delay(200);
        
    } else {
        float state_left = get_IR_dist(IR_LEFT);
        float state_right = get_IR_dist(IR_RIGHT);
        p_controller_run(KP_LEFT, KP_RIGHT, KI_LEFT, KI_RIGHT, state_left, state_right, SETPOINT_LEFT, SETPOINT_RIGHT, BASE_SPEED);
    }
}

// function for the PID controller
void p_controller_run(float kp_left, float kp_right, float ki_left, float ki_right, float state_left, float state_right, float setpoint_left, float setpoint_right, int base_speed) {
    float error_left = state_left - setpoint_left;
    integral_left = integral_left + (error_left * 0.01);
    float output_left = kp_left * error_left + ki_left * integral_left;
    float error_right = state_right - setpoint_right;
    integral_right = integral_right + (error_right * 0.01);
    float output_right = kp_right * error_right + ki_right * integral_right;

    // convert output into change in motor speeds
    int speed_left = (base_speed + output_right) * (-1);
    int speed_right = (base_speed + output_left);
    Serial.print("LEFT: ");
    Serial.println(speed_left);
    Serial.print("RIGHT: ");
    Serial.println(speed_right);
    motor_left.run(speed_left);
    motor_right.run(speed_right);
}

// function to get the distance of the vehicle from the wall
float get_IR_dist(int pin) {
    int raw = analogRead(pin);
    return raw;
}
