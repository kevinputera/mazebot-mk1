#include "MeMCore.h"

MeRGBLed onboard_led(PORT_7);

void setup() {
  
}

void loop() {
  onboard_led.setColorAt(1, 50, 0, 0);
  onboard_led.show();
}
