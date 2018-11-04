#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define LOW_FREQ_FILTER A2
#define HIGH_FREQ_FILTER A3
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
// initialize ultrasonic sensor object
MeUltrasonicSensor ultrasonic_sensor(PORT_2);
// initialize RGB LED object
MeRGBLed rgb_led(PORT_7);
// initialize light sensor(LDR) object
MeLightSensor light_sensor(PORT_8);

// declare global variables for the PID controller
int integral = 0;
int error_prior = 0;

void setup() {
    // initialize left and right IR sensor pins
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);

    // initialize high and low frequency filter pins
    pinMode(LOW_FREQ_FILTER, INPUT);
    pinMode(HIGH_FREQ_FILTER, INPUT);

    // initialize start button
    // press then release button to begin the run
    pinMode(BUTTON, INPUT);
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}
}

void loop() {
    // run wall following so long as no black line is detected
    if (line_detector.readSensor1 == 1) {
        // pid controller code here!
        int state = get_IR_dist(IR_LEFT);
        p_controller_run(KP, state, SETPOINT, BASE_SPEED);

        // skips the rest of the code
        return;
    }

    //stop both motors
    motor_left.stop();
    motor_right.stop();

    // check whether there is a color challenge
    // if yes, solve it, do the necessary movement, call return;

    // check whether there is a sound challenge
    // if yes, solve it, do the necessary movement, call return;

    // if the loop reaches this point, end the run
    // play celebration tune and disco LED
    ending();
}

// function to get the color codes(in integers) from the color challenge
/* RED -> return 0
   GREEN -> return 1
   WHITE -> return 2
   ORANGE -> return 3
   BLUE -> return 4 
   NO COLOR CHALLENGE -> return -1 */
int get_color_code() {

}

// function to compare the difference in loudness between sound A of freq. 300Hz and sound B of freq. 3000Hz
/* A > B -> return 0
   A < B -> return 1
   A = B -> return 2
   NO SOUND CHALLENGE -> return -1 */
int get_sound_diff() {

}

// function to turn the robot after the challenges
// do the turn according to the return values of get_sound_diff() and get_color_code()
/* code = 0 -> left turn
   code = 1 -> right turn
   code = 2 -> 180 turn within the same grid
   code = 3 -> two successive left turns in two grids
   code = 4 -> two successive right turns in two grids
   code = -1 -> no turn */
void turn(int code) {

}

// function for the P controller
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

// function to play celebration tune and disco LED
void ending() {
    
}