
// see locking sketch for explanation:
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
  // put your setup code here, to run once:

  Serial.begin(9600);
  setPwmFrequency(9,1);

}

void loop() {
  // put your main code here, to run repeatedly:

  int wPin = 9;
  int rPin = A5;
  
  for (int x=0; x<255; x++){
    analogWrite(wPin,x);
    Serial.print(analogRead(rPin));Serial.print(", ");
    Serial.println();
    delay(100);
    analogWrite(wPin,0);
    delay(100);
  }
  
  analogWrite(wPin,64);
  delay(2000);
  analogWrite(wPin,128);
  delay(2000);
  analogWrite(wPin,192);
  delay(2000);
  
  //int val = analogRead(A5);
  //Serial.println(val);

}
