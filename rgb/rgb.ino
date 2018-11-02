#include "MeMCore.h"

MeRGBLed onboard_led(PORT_7);

void setup() {
  
}

void loop() {
  onboard_led.setColor(50, 0, 0);
  onboard_led.show();
  delay(50);
  onboard_led.setColor(0, 50, 0);
  onboard_led.show();
  delay(50);
  onboard_led.setColor(0, 0, 50);
  onboard_led.show();
  delay(50);
}
