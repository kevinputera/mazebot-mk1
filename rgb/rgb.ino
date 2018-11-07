#include <MeMCore.h>
// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 50 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 

// Define colour sensor LED pins
int ledArray[] = {2, 3, 4};

//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

//floats to hold colour arrays
float colourArray[] = {0, 0, 0};
float whiteArray[] = {0, 0, 0};
float blackArray[] = {0, 0, 0};
float greyDiff[] = {0, 0, 0};

char colourStr[3][5] = {"R = ", "G = ", "B = "};

MeDCMotor motor1(M1); //Motor1 is Left Motor
MeDCMotor motor2(M2); //Motor2 is Right Motor
MeLightSensor lightSensor(PORT_6);
MeRGBLed led(0, 30);

void setBalance() {
  //set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           //delay for five seconds for getting sample ready
  led.setColor(0, 0, 0);
  led.show();//Check Indicator OFF during Calibration
  //scan the white sample.
  //go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  led.setColor(255, 0, 0);
  led.show();
  delay(RGBWait);
  whiteArray[0] = getAvgReading(5);         //scan 5 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);

  led.setColor(0, 255, 0);
  led.show();
  delay(RGBWait);
  whiteArray[1] = getAvgReading(5);         //scan 5 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);

  led.setColor(0, 0, 255);
  led.show();
  delay(RGBWait);
  whiteArray[2] = getAvgReading(5);         //scan 5 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);
  //done scanning white, time for the black sample.
  //set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready
  //go through one colour at a time, set the minimum reading for red, green and blue to the black array
  led.setColor(255, 0, 0);
  led.show();
  delay(RGBWait);
  blackArray[0] = getAvgReading(5);         //scan 5 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);

  led.setColor(0, 255, 0);
  led.show();
  delay(RGBWait);
  blackArray[1] = getAvgReading(5);         //scan 5 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);

  led.setColor(0, 0, 255);
  led.show();
  delay(RGBWait);
  blackArray[2] = getAvgReading(5);         //scan 5 times and return the average,
  led.setColor(0, 0, 0);
  led.show();
  delay(RGBWait);
  //the differnce between the maximum and the minimum gives the range
  for (int i = 0; i <= 2; i++ ) {
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }

  //delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  delay(5000);
}

int getAvgReading(int times) {
  //find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  //take the reading as many times as requested and add them up
  for (int i = 0; i < times; i++) {
    reading = lightSensor.read();
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
  return total / times;
}

void setup()
{
  Serial.begin(9600);
  led.setpin(13);
  setBalance();
  led.setColor(255, 255, 255);
  led.show();

}

void loop()
{
  //turn on one colour at a time and LDR reads 5 times

  Serial.print(colourStr[0]);
  led.setColor(255, 0, 0);
  led.show(); //turn ON the LED, red, green or blue, one colour at a time.
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average
  colourArray[0] = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[0] = (colourArray[0] - blackArray[0]) / (greyDiff[0]) * 255;
  led.setColor(0, 0, 0);
  led.show(); //turn off the current LED colour
  delay(RGBWait);
  Serial.println(int(colourArray[0])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code

  Serial.print(colourStr[1]);
  led.setColor(0, 255, 0);
  led.show(); //turn ON the LED, red, green or blue, one colour at a time.
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average
  colourArray[1] = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[1] = (colourArray[1] - blackArray[1]) / (greyDiff[1]) * 255;
  led.setColor(0, 0, 0);
  led.show(); //turn off the current LED colour
  delay(RGBWait);
  Serial.println(int(colourArray[1])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code


  Serial.print(colourStr[2]);
  led.setColor(0, 0, 255);
  led.show(); //turn ON the LED, red, green or blue, one colour at a time.
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average
  colourArray[2] = getAvgReading(5);
  //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
  colourArray[2] = (colourArray[2] - blackArray[2]) / (greyDiff[2]) * 255;
  led.setColor(0, 0, 0);
  led.show(); //turn off the current LED colour
  delay(RGBWait);
  Serial.println(int(colourArray[2])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code

  if (colourArray[2] > colourArray[1] && colourArray[2] > colourArray[0]) {
    Serial.println("BLUE");
  }
  else if (colourArray[1] > colourArray[2] && colourArray[1] > colourArray[0]) {
    Serial.println("GREEN");
  }
  else if ((colourArray[1] + colourArray[2]) / colourArray[0] > 0.5) {
    Serial.println("RED");
  }
  else if (colourArray[1] < 50 && colourArray[2] < 50 && colourArray[0] < 50) {
    Serial.println("BLACK");
  }
  else if ((colourArray[1] + colourArray[2]) / colourArray[0] < 0.5) {
    Serial.println("ORANGE");
  }
}
/*
  Serial.print("value = ");           // Print the results to the serial monitor
  Serial.println(lightSensor.read()); // Brightness value from 0-1023
  delay(50);                          // Wait 50 milliseconds before next measurement
*/
