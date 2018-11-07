#define IR_LEFT A0
#define IR_RIGHT A1

void setup() {
  // put your setup code here, to run once:
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    float x_right = analogRead(IR_RIGHT);
    float output_right = 2.275427 - (0.00003065916/0.009841547)*(1 - pow(EULER,(0.009841547*x_right)));
    Serial.println(output_right);

    float x_left = analogRead(IR_LEFT);
    float output_left = 2.208064 - (0.00005070831/0.0100127)*(1 - pow(EULER,(0.0100127*x_left)));
    Serial.println(output_left);
}
