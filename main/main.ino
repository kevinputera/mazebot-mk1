void setup() {
  // setup directives.
  pinMode(9, OUTPUT);
}

void loop() {
  // Run PID controller.
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(1000);


}
