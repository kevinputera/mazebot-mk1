#include <MeMCore.h>

#define WHITE_GREEN 309
#define BLACK_GREEN 54
#define GREY_GREEN 255

#define WHITE_UPPER 800
#define WHITE_LOWER 520
#define BLACK_UPPER 90
#define RED_UPPER 110
#define RED_LOWER 90
#define ORANGE_UPPER 180
#define ORANGE_LOWER 110
#define GREEN_UPPER 265
#define GREEN_LOWER 200
#define BLUE_UPPER 520
#define BLUE_LOWER 350

//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

MeLightSensor lightSensor(PORT_6);
MeRGBLed led(PORT_7);

void setBalance() {
  //set white balance
  led.setColor(0, 255, 255);
  led.show();
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           //delay for five seconds for getting sample ready
  int white = getAvgReading(5);         //scan 5 times and return the average,
  
  //set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready
  int black = getAvgReading(5);         //scan 5 times and return the average,
  
  //the differnce between the maximum and the minimum gives the range
  int grey = white - black;

  //print the white, black and grey
  Serial.print("WHITE: ");
  Serial.println(white);
  Serial.print("BLACK: ");
  Serial.println(black);
  Serial.print("GREY: ");
  Serial.println(grey);
}

int getAvgReading(int times) {
  //find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  //take the reading as many times as requested and add them up
  for (int i = 0; i < times; i++) {
    reading = lightSensor.read();
    total = reading + total;
    delay(20); 
  }
  //calculate the average and return it
  return total / times;
}

void setup()
{
  Serial.begin(9600);
  //setBalance();
  led.setColor(0, 255, 255);
  led.show();
  //while (true) {}
}

void loop()
{
  delay(100);
  //get the average of 5 consecutive readings for the current colour and return an average
  float color = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  color = (color - BLACK_GREEN) / (GREY_GREEN) * 255;
  Serial.println(color);

    if (color > WHITE_LOWER && color < WHITE_UPPER) {
        Serial.println("white");
    } else if (color < BLACK_UPPER) {
        Serial.println("black");
    } else if (color > RED_LOWER && color < RED_UPPER) {
        Serial.println("red");
    } else if (color > ORANGE_LOWER && color < ORANGE_UPPER) {
        Serial.println("orange");
    } else if (color > GREEN_LOWER && color < GREEN_UPPER) {
        Serial.println("green");
    } else if (color > BLUE_LOWER && color < BLUE_UPPER) {
        Serial.println("blue");
    }
}
