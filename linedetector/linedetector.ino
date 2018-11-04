#include "MeMCore.h"

MeLineFollower line_detector(PORT_1);

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println(line_detector.readSensor1());
}
