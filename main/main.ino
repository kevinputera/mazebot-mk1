#include <MeMCore.h>

// define pin numbers for multiple sensors
#define IR_LEFT A0
#define IR_RIGHT A1
#define LOW_FREQ_FILTER A2
#define HIGH_FREQ_FILTER A3
#define BUTTON A7

// define constants for on/off controller
#define SETPOINT_LEFT 600
#define SETPOINT_RIGHT 580
#define TOLERANCE 5
#define BASE_SPEED 255

// define delays
#define DELAY_AVG_LIGHT 10 // delay for the get_avg_reading function
#define DELAY_CHALLENGE 100 // delay before solving the challenges
#define DELAY_TURN 100 // delay after turning
#define DELAY_BLINK 70

// define turn waits (how long should the motors run for turns)
#define RIGHT_TURN 275
#define LEFT_TURN 270
#define U_TURN 745
#define STRAIGHT 610

// define color reading boundaries
#define WHITE_UPPER 854
#define WHITE_LOWER 574
#define BLACK_UPPER 144
#define RED_UPPER 194
#define RED_LOWER 144
#define ORANGE_UPPER 234
#define ORANGE_LOWER 194
#define GREEN_UPPER 319
#define GREEN_LOWER 254
#define BLUE_UPPER 574
#define BLUE_LOWER 404

// define number of samples taken
#define NUM_OF_SOUND_SAMPLES 200
#define NUM_OF_LIGHT_SAMPLES 5

// define the frequency for the notes used in the ending tune
#define NOTE_E6  1319
#define NOTE_G6  1568
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136
#define NOTE_A7  3520

// initialize peripheral objects
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
MeLineFollower line_detector(PORT_1);
MeUltrasonicSensor ultrasonic_sensor(PORT_2);
MeRGBLed rgb_led(PORT_7);
MeLightSensor light_sensor(PORT_8);
MeBuzzer buzzer;

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

    // turn off the LED
    rgb_led.setColor(0, 0, 0);
    rgb_led.show();
    
    while (analogRead(BUTTON) > 500) {}
    while (analogRead(BUTTON) < 500) {}

    // turn on the LED
    rgb_led.setColor(0, 255, 255);
    rgb_led.show();
}

void loop() {
    // run wall following so long as no black line is detected
    if (line_detector.readSensor1() == 1) {
        // on/off controller
        int state_left = get_IR_reading(IR_LEFT);
        int state_right = get_IR_reading(IR_RIGHT);
        bangbang(state_left, state_right);
    } else {
        // encountered black line
        // stop both motors
        motor_left.stop();
        motor_right.stop();
        
        // check sound and color challenge
        delay(DELAY_CHALLENGE);
        int turn_code = get_color_code();
        while (turn_code == -1) {
            // repeat color detection if color is not recognized
            turn_code = get_color_code();
        }

        // if the color black is detected, check whether it is a sound challenge
        if (turn_code == 5) {
            turn_code = get_sound_code();
            if (turn_code == -1) {
                // if the color black is detected and no sound challenge, end the run
                // put the maze into infinite loop to end the run
                ending();
            }
        }
        
        // turn the vehicle according to the instruction
        turn(turn_code);
    }
}

// function to get the average LDR reading
// to be used in the get_color_code function
int get_light_avg(int times) {
    int reading;
    int total = 0;
    for (int i = 0; i < times; i += 1) {
        delay(DELAY_AVG_LIGHT);
        reading = light_sensor.read();
        total = reading + total;
    }
    return (total / times);
}

// function to get the avg 3000Hz signal reading
float get_thousand_signal(int times) {
    float thousandValue = 0;
    for (int i = 0; i < times; i += 1) {
        thousandValue += analogRead(HIGH_FREQ_FILTER);
    }
    return (thousandValue / times);
}

// function to get the avg 300Hz signal reading
float get_hundred_signal(int times) {
    float hundredValue = 0;
    for (int i = 0; i < times; i += 1) {
        hundredValue += analogRead(LOW_FREQ_FILTER);
    }
    return (hundredValue / times);
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
    int color = get_light_avg(NUM_OF_LIGHT_SAMPLES);

    // get color codes
    if (color > WHITE_LOWER && color < WHITE_UPPER) {
        // WHITE
        return 2;
    } else if (color < BLACK_UPPER) {
        // BLACK
        return 5;
    } else if (color > RED_LOWER && color < RED_UPPER) {
        // RED
        return 0;
    } else if (color > ORANGE_LOWER && color < ORANGE_UPPER) {
        // ORANGE
        return 3;
    } else if (color > GREEN_LOWER && color < GREEN_UPPER) {
        // GREEN
        return 1;
    } else if (color > BLUE_LOWER && color < BLUE_UPPER) {
        // BLUE
        return 4;
    }
    // fail to detect
    return -1;
}

// function to compare the difference in loudness between sound A of freq. 300Hz and sound B of freq. 3000Hz
/* A > B -> return 0
   A < B -> return 1
   A = B -> return 2
   NO SOUND CHALLENGE -> return -1 */
int get_sound_code() {
    float thousand_signal = get_thousand_signal(NUM_OF_SOUND_SAMPLES);
    float hundred_signal = get_hundred_signal(NUM_OF_SOUND_SAMPLES);
    float ratio = hundred_signal / thousand_signal;
 
    if (hundred_signal < 50 && thousand_signal < 50) {
        // no sound challenge
        return -1;
    }
    if (ratio > 5){
        // freq. of A < freq. of B
        return 1;
    }
    if (ratio < 0.35){
        // freq. of A > freq. of B
        return 0;
    }
    // freq. of A == freq. of B
    return 2;
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
        motor_left.run(-150);
        motor_right.run(-200);
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

// function for the on/off controller
void bangbang(int state_left, int state_right) {
    if (state_left < (SETPOINT_LEFT - TOLERANCE)) {
        // turn right
        motor_left.run(-255);
        motor_right.run(50);
    } else if (state_right < (SETPOINT_RIGHT - TOLERANCE)) {
        // turn left
        motor_left.run(-50);
        motor_right.run(255);
    } else {
        // move forward
        motor_left.run(-255);
        motor_right.run(240);
    }
}

// function to get the IR readings
int get_IR_reading(int pin) {
    int raw = analogRead(pin);
    return raw;
}

// function to play ending tune
void ending() {
    // set the tones
    int melody[] = {
        NOTE_E7, NOTE_E7, 0, NOTE_E7,
        0, NOTE_C7, NOTE_E7, 0,
        NOTE_G7, 0, 0,  0,
        NOTE_G6, 0, 0, 0,

        NOTE_C7, 0, 0, NOTE_G6,
        0, 0, NOTE_E6, 0,
        0, NOTE_A6, 0, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,

        NOTE_G6, NOTE_E7, NOTE_G7,
        NOTE_A7, 0, NOTE_F7, NOTE_G7,
        0, NOTE_E7, 0, NOTE_C7,
        NOTE_D7, NOTE_B6, 0, 0,

        NOTE_C7, 0, 0, NOTE_G6,
        0, 0, NOTE_E6, 0,
        0, NOTE_A6, 0, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,

        NOTE_G6, NOTE_E7, NOTE_G7,
        NOTE_A7, 0, NOTE_F7, NOTE_G7,
        0, NOTE_E7, 0, NOTE_C7,
        NOTE_D7, NOTE_B6, 0, 0};

    // set the durations of the tones
    int note_duration[] = {
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        9, 9, 9,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        9, 9, 9,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12};

    while (true) {
        // leave the microcontroller in infinite loop
        // press the reset button to exit
        int size_melody = sizeof(melody) / sizeof(int);
        for (int i = 0; i < size_melody; i += 1) {
            int duration = 1000 / note_duration[i];
            buzzer.tone(8, melody[i], duration);
            int pause = duration * 1.5;
            delay(pause);
            buzzer.noTone(8);
        }
        delay(300);
    }
}
