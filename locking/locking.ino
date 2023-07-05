/* About this Sketch
 * -----------------
 * 
 *  We use this sketch to lock an interferometer to a fixed operating point over a long period of time. 
 *  This sketch is designed to be loaded up to an Arduino Uno. 
 *  If another microcontroller is used, maybe other pins have to be used.
 *  Additional hardware: interferometer, piezo for mirror, pwm-lowpass.
 *  
 *  More information can be found in the additional files also contained in the repository.
 *  
 *  Convention in calculations: uncapitalized - floats or auxilliary (e.g. index); capitalized - ints.
 */

// declare Pins
int rPin = A5;  // The photodiode should be connected to this pin. Has to be an analog pin.
int wPin = 9;   // On an Arduino Uno it is best to use pin 9. See the comments to the function setPwmFrequency for more details.

// PI settings
float p = .1;      // This coefficient for the proportional control has to be adjusted to the system.
float i = .3;      // This coefficient for the integral control has to be adjusted to the system.
int numint = 20;   // Defines how far in the past the integral starts.
int PZzero = 128;  // Has to be in 0..255 - best in the center.
float minimal_slope = 0.01;

// additional global PID-variables 
float slope = 0.0;
int PD_goal;
int Lastvals[20];  // Length should be the same as numint.
int counter = 0;


/* setPwmFrequency
 * ---------------
 * 
 *  Part of this code tries to use the analogWrite() function in a more customized manner.
 *  analogWrite(pin, val) uses Pulse Width Modulation, that means it creates a steady rectangular wave of the duty cycle specified by val/255
 *  at the specified pin until its next call with that pin. The basic frequency of this rectangular wave is dependent on the pin.
 *  It can be manipulated in manipulating the internal timers of the microcontroller. This is what the following function does.
 *  It is copied from that article: https://playground.arduino.cc/Code/PwmFrequency/
 *  I want to get a higher basic frequency because the default ones are slow. Maybe they would be fast enough but for this sketch there should be no harm in speeding them up.
 *  To convert the signal from pulse width modulation to a steady voltage of corresponding magnitude, you need to integrate it with a low-pass filter. 
 *  
 *  In the arduino reference to the analogWrite() function [ https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/ ]
 *  it says, you should not use PIN 5 or 6 if you need preicse handling of small values.
 *  So I change to PIN 9 and divisor 1 which should give me a base frequency of 31250 Hz.
 */

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


void setup() {

  // initialize
  setPwmFrequency(9,1);       // sets the basic pwm-frequency at Pin9 to about 31.2 kHz
  Serial.begin(9600);
  memset(Lastvals,0,numint);  // sets all entries of Lastvals to 0 (maybe unnecessary)

  // get actual slope
  while(abs(slope)<minimal_slope){
    Serial.println("Startup...");
    analogWrite(wPin,PZzero-24);
    delay(100);
    int PD00 = analogRead(rPin);
    analogWrite(wPin,PZzero+24);
    delay(100);
    int PD01 = analogRead(rPin);
    slope = float(PD01-PD00)/48.;
    Serial.print("Actual slope is: ");Serial.println(slope);
  }

  // get PD_goal to hold on to ('setpoint'; could also be hard-coded to e.g. 128)
  analogWrite(wPin,PZzero); delay(100);
  PD_goal = analogRead(rPin);

  // prepare Serial Plotter
  Serial.print("PD_goal");Serial.print(",");
  Serial.print("PD_act");Serial.print(",");
  Serial.print("average");Serial.print(",");
  Serial.println("PZ");
  
}


void loop() {  
  
  // read actual value
  int PD_act = analogRead(rPin);
  int Diff = PD_act-PD_goal;
  Lastvals[counter] = Diff;
  counter++;
  if(counter>=numint){counter = 0;}
  
  // calculate average of last x Diffs; X = numint;
  int Sum = 0;
  for (int j = 0; j<numint; j++){Sum += Lastvals[j];}
  float average = float(Sum)/float(numint);            // note: adding integers should be way faster than floats -> convert afterwards

  // calculate Piezo control value
  static int PZ = PZzero;  // static, cause it should keep its value between calls of loop, so that corrections (PI) are from the last actual value. This line only gets executed in the first call of the loop.
  pz = float(PZ)-(p*float(Diff)+i*average)*slope;
  PZ = round(pz)
  if(PZ<0){PZ=0;}
  if(PZ>255){PZ=255;}
  analogWrite(wPin, PZ);
  
  // print debug infos
  Serial.print(PD_goal);Serial.print(",");
  Serial.print(PD_act);Serial.print(",");
  Serial.print(average);Serial.print(",");
  Serial.println(PZ);
  
}
