int SENSOR_PIN_THOUSAND = A2; //Change pin according to needs
int SENSOR_PIN_HUNDRED = A3; //Change pin according to needs
int NUMBER_OF_SOUND_SAMPLES = 200;
long get_thouand_signal();
long get_hundred_signal();


#define OFFSET_IN_HUNDRED_SIGNAL 84


void setup() {
  Serial.begin(9600);
}

long get_thousand_signal() {
  long thousandValue = 0;
  for (int i = 0; i < NUMBER_OF_SOUND_SAMPLES; i += 1) {
    thousandValue += analogRead(SENSOR_PIN_THOUSAND);
  }

  thousandValue /= NUMBER_OF_SOUND_SAMPLES;
  //Serial.println(thousandValue);
  return thousandValue;
}

long get_hundred_signal() {
  long hundredValue = 0;
  for (int i = 0; i < NUMBER_OF_SOUND_SAMPLES; i += 1) {
    hundredValue += analogRead(SENSOR_PIN_HUNDRED);
  }

  hundredValue /= NUMBER_OF_SOUND_SAMPLES;
  return hundredValue;
}

int get_sound_challenge(long thousand_signal, long hundred_signal){ //hundred is higher turn left return 2; if signals have similar loudness return 1; else thousand is louder return 0
  double ratio = (double) hundred_signal / thousand_signal;
  if (hundred_signal < 100 && thousand_signal < 100) {
    return -1;
  }
  if(ratio > 5){
    return 0;
  }
  else if(ratio < 0.5){
    return 1;
  }
  return 2;
}


void loop() {
    int a = get_sound_challenge(get_thousand_signal(),get_hundred_signal());
    Serial.println(a);
}
