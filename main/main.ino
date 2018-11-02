#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define LOW_FREQ_FILTER A2
#define HIGH_FREQ_FILTER A3
#define BUTTON A7
// define constants for PID controller
#define KP 1
#define KI 0
#define KD 0
#define TIME_INTERVAL 0.01
#define SETPOINT 500

// initialize left and right motor objects
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
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
        pid_run(KP, KI, KD, state, SETPOINT, TIME_INTERVAL);

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

// function for the PID controller
void pid_run(int Kp, int Ki, int Kd, int state, int setpoint, int time_interval) {
    int error = state - setpoint;
    integral = integral + (error * time_interval);
    int derivative = (error - error_prior) / time_interval;
    int output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // convert output into change in motor speeds

    error_prior = error;
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