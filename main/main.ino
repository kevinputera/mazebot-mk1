#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define LOW_FREQ_FILTER A2
#define HIGH_FREQ_FILTER A3
#define BUTTON A7

// define constants for PI controller
#define KP_LEFT 2.3
#define KP_RIGHT 2.3
#define KI_LEFT 0.1
#define KI_RIGHT 0.1
#define SETPOINT_LEFT 640
#define SETPOINT_RIGHT 640
#define BASE_SPEED 255
#define INTERVAL 0.01

// define delays
#define DELAY_AVG 20 // delay for the get_avg_reading function
#define DELAY_COLOR 100 // delay for the get_color_code function
#define DELAY_TURN 100

// define turn waits (how long should the motors run for turns)
#define RIGHT_TURN 290
#define LEFT_TURN 290
#define U_TURN 580
#define STRAIGHT 700

// define color calibration values
#define WHITE 384
#define BLACK 53
#define GREY 331

// define color reading boundaries
#define WHITE_UPPER 280
#define WHITE_LOWER 220
#define BLACK_UPPER 10
// #define BLACK_LOWER (not used since black produces the lowest value)
#define RED_UPPER 15
#define RED_LOWER 28
#define ORANGE_UPPER 32
#define ORANGE_LOWER 55
#define GREEN_UPPER 80
#define GREEN_LOWER 97
#define BLUE_UPPER 97
#define BLUE_LOWER 130

// initialize peripheral objects
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
MeLineFollower line_detector(PORT_1);
MeUltrasonicSensor ultrasonic_sensor(PORT_2);
MeRGBLed rgb_led(PORT_7);
MeLightSensor light_sensor(PORT_8);

// declare global variables for the PI controller
float integral_left = 0;
float integral_right = 0;

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
    if (line_detector.readSensor1() == 1) {
        // PI controller code here!
        int state_left = get_IR_reading(IR_LEFT);
        int state_right = get_IR_reading(IR_RIGHT);
        follow_wall(KP_LEFT, KP_RIGHT, KI_LEFT, KI_RIGHT, state_left, state_right, SETPOINT_LEFT, SETPOINT_RIGHT, BASE_SPEED);
    } else {
        // check sound challenge

        // solve sound challenge
        
        // solve color challenge
        int turn_code = get_color_code();
        while (turn_code == -1) {
            // loop to make sure to get a color code
            turn_code = get_color_code();
        }

        // turn the vehicle according to the instruction
        turn(turn_code);
        
        // reset integral values
        integral_left = 0;
        integral_right = 0;
    }
}

// function to get the average LDR reading
// to be used in the get_color_code function
int get_avg_reading(int times) {
    int reading = 0;
    int total = 0;

    for (int i = 0; i < times; i += 1) {
        delay(DELAY_AVG)
        reading = light_sensor.read();
        total = reading + total;
    }
    return (total / times);
}

// function to get the color codes(in integers) from the color challenge
// use only green LED
/* RED -> return 0
   GREEN -> return 1
   WHITE -> return 2
   ORANGE -> return 3
   BLUE -> return 4 
   BLACK -> return 5
   OTHERS -> return -1 */
int get_color_code() {
    delay(DELAY_COLOR);
    int reading = get_avg_reading(5);
    float color = (reading - BLACK) / GREY * 255;

    // get color codes
    if (color > WHITE_LOWER && color < WHITE_UPPER) {
        return 2;
    } else if (color < BLACK_UPPER) {
        return 5;
    } else if (color > RED_LOWER && color < RED_UPPER) {
        return 0;
    } else if (color > ORANGE_LOWER && color < ORANGE_UPPER) {
        return 3;
    } else if (color > GREEN_LOWER && color < GREEN_UPPER) {
        return 1;
    } else if (color > BLUE_LOWER && color < BLUE_UPPER) {
        return 4;
    }
    return -1;
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
    if (code == 0) {
        // turn left
        motor_left.run(255);
        motor_right.run(255);
        delay(LEFT_TURN);
        motor_left.stop();
        motor_right.stop();

    } else if (code == 1) {
        // turn right
        motor_left.run(-255);
        motor_right.run(-255);
        delay(RIGHT_TURN);
        motor_left.stop();
        motor_right.stop();

    } else if (code == 2) {
        // turn 180 (u-turn)
        motor_left.run(255);
        motor_right.run(255);
        delay(U_TURN);
        motor_left.stop();
        motor_right.stop();

    } else if (code == 3) {
        // two successive left turns
        // turn left
        motor_left.run(255);
        motor_right.run(255);
        delay(LEFT_TURN);
        motor_left.stop();
        motor_right.stop();
        delay(DELAY_TURN);

        // move straight
        motor_left.run(-255);
        motor_right.run(255);
        delay(STRAIGHT);
        motor_left.stop();
        motor_right.stop();
        delay(DELAY_TURN);

        // turn left
        motor_left.run(255);
        motor_right.run(255);
        delay(LEFT_TURN);
        motor_left.stop();
        motor_right.stop();

    } else if (code == 4) {
        // two successive right turns
        // turn right
        motor_left.run(-255);
        motor_right.run(-255);
        delay(RIGHT_TURN);
        motor_left.stop();
        motor_right.stop();
        delay(DELAY_TURN);

        // move straight
        motor_left.run(-255);
        motor_right.run(255);
        delay(STRAIGHT);
        motor_left.stop();
        motor_right.stop();
        delay(DELAY_TURN);

        // turn right
        motor_left.run(-255);
        motor_right.run(-255);
        delay(RIGHT_TURN);
        motor_left.stop();
        motor_right.stop(); 

    }
    delay(DELAY_TURN);
}

// function for the PI controller
void follow_wall(float kp_left, float kp_right, float ki_left, float ki_right, int state_left, int state_right, int setpoint_left, int setpoint_right, int base_speed) {
    float error_left = state_left - setpoint_left;
    integral_left = integral_left + (error_left * INTERVAL);
    float output_left = kp_left * error_left + ki_left * integral_left;

    float error_right = state_right - setpoint_right;
    integral_right = integral_right + (error_right * INTERVAL);
    float output_right = kp_right * error_right + ki_right * integral_right;

    // convert output into change in motor speeds
    int speed_left = (base_speed + output_right) * (-1);
    int speed_right = (base_speed + output_left);

   /* Serial.print("LEFT: ");
    Serial.println(speed_left);
    Serial.print("RIGHT: ");
    Serial.println(speed_right); */

    motor_left.run(speed_left);
    motor_right.run(speed_right);
}

// function to get the distance of the vehicle from the wall
// we'll use the value 0-1023 to represent the distance
int get_IR_reading(int pin) {
    int raw = analogRead(pin);
    return raw;
}

// function to play celebration tune and disco LED
void ending() {
    
}
