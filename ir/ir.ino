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
    int output_right = analogRead(IR_RIGHT);
    Serial.print("RIGHT: ");
    Serial.println(output_right);

    int output_left = analogRead(IR_LEFT);
    Serial.print("LEFT: ");
    Serial.println(output_left);
}
