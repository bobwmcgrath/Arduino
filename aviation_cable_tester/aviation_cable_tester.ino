//This code runs on an arduino uno and is used to test 2 pin and 4 pin aviation cables with a custom shield
//by Bob McGrath 2/26/19
int timer = 10;           // The higher the number, the slower the timing.

int recievePins[] = {
  14, 15, 16, 17, 18, 19
};      

int testPins[] = {
   9, 8, 10, 11, 12, 13
};  

int pinState[] = {
  1, 2, 3, 4, 5, 6
};   

int pinCount = 6; 




void setup() {
  Serial.begin(115200);
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(testPins[thisPin], OUTPUT);
  }

  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(testPins[thisPin], LOW);
    delay(timer);}
  
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(recievePins[thisPin], INPUT_PULLUP);
  }

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);


}
void greenLight() {
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
}
void redLight() {
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
}
void offLight() {
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
}


void loop() {

    // loop from the lowest pin to the highest:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(testPins[thisPin], LOW);
    delay(timer);
    pinState[thisPin]=digitalRead(recievePins[thisPin]);
    // turn the pin off:
    digitalWrite(testPins[thisPin], HIGH);
    delay(timer);
    //Serial.println(recievePins[thisPin]);
  }
if (
pinState[0]==0 && 
pinState[1]==0 && 
pinState[2]==0 && 
pinState[3]==0 && 
pinState[4]==1 && 
pinState[5]==1)
greenLight();

else if (
pinState[0]==1 && 
pinState[1]==1 && 
pinState[2]==1 && 
pinState[3]==1 && 
pinState[4]==0 && 
pinState[5]==0)
greenLight();

else if (
pinState[0]==1 && 
pinState[1]==1 && 
pinState[2]==1 && 
pinState[3]==1 && 
pinState[4]==1 && 
pinState[5]==1)
offLight();

else redLight();

for (int thisPin = 0; thisPin < pinCount; thisPin++){
Serial.print(pinState[thisPin]);
}
Serial.println();
}
