 /* roboshelf by Bob*/
#include <EEPROM.h>
//#include <OneWire.h>
#include "CONST.h"
#include "OWB.h"
#include "PINS.h"
#include "INI.h"

using namespace owb;
using namespace pins;

void setup()
{
  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//} 

 owb::OWB_setup();

  
// Configure Timer 1 for PWM @ 25 kHz.
    TCCR1A = 0;           // undo the configuration done by...
    TCCR1B = 0;           // ...the Arduino core library
    TCNT1  = 0;           // reset timer
    TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
           | _BV(COM1B1)  // same on ch; B
           | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
    TCCR1B = _BV(WGM13)   // ditto
           | _BV(CS10);   // prescaler = 1
    ICR1   = 320;         // TOP = 320

 pins::pinINI();
 attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
 EEPROM.get(0,teach_encoder0Pos);
 sense();

Serial.println("setup");
//encoder0Pos=0;
}

//AN ANALOG WRITE THAT PWMS AT FASTER THAN THE NORMAL 490HZ
void analogWrite25k(int pin, int value)
{
    switch (pin) {
        case 9:
            OCR1A = value;
            break;
        default:
            // no other pin will work
            break;
    }
}

void doEncoder() {
  if (GO_DOWN_FLAG==1) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }

  //Serial.println (encoder0Pos, DEC);
}



void goUp(int spd, int x)
{
 GO_DOWN_FLAG=0;
 //sense();
 //if (BUTTON_STOP_STATE == 0){   
 digitalWrite(IN1,LOW);// rotate forward
 digitalWrite(IN2,HIGH);
 analogWrite25k(ENA, spd);// motor speed  
 //Serial.println("up");
 delay(x);
  
 //}
} 

void goDown(int spd, int x)
{ 
 GO_DOWN_FLAG=1;  
 digitalWrite(IN1,HIGH);// rotate reverse
 digitalWrite(IN2,LOW);
 analogWrite25k(ENA, spd);// motor speed  
 delay(x);
// if US_DISTANCE-LAST_US_DISTANCE>
 //Serial.println("down");
 //Serial.println(spd);
} 

void goTo(){

while (teach_encoder0Pos>encoder0Pos){
if (acc<300&&teach_encoder0Pos-encoder0Pos>1000)acc+=5;
else if (acc>50)acc-=5;
if (BUTTONS==BUTTONS_INI) buttonsFlag=1;
if (BUTTONS==GO_B&&buttonsFlag==1){
  while (BUTTONS==GO_B){
    delay(10);
    sense();
    }
  return 0; 
}
goDown(acc,10);
sense();}
  
}

void goHome(){
  while (BUTTON_STOP_STATE==1){
  if (acc<300&&encoder0Pos>1000)acc+=5;
  else if (acc>50)acc-=5;
  if (BUTTONS==goHome_B) buttonsFlag=1;
  if (BUTTONS==goHome_B&&buttonsFlag==1){
    analogWrite25k(ENA, 0);
    while (BUTTONS==goHome_B){
      delay(10);
      sense();
    }
    return 0;
  }
  sense();
  goUp(acc,10);
  //Serial.println("home");
  }
}
void teach(){ 
 delay(10);
 while (BUTTONS==TEACH_B){
    goDown(acc,100);
    sense();
    if (acc<300)acc+=10;
    }
    goDown(0,100);
    delay(dly);
  teach_encoder0Pos=encoder0Pos;
  EEPROM.put(0,teach_encoder0Pos);
}

void sense(){
  delay(10);
  BUTTONS = owb::read();
  delay(10);
  BUTTON_STOP_STATE=digitalRead(BUTTON_STOP);
  AMPS_STATE=analogRead(A0);
  Serial.println(BUTTONS,BIN);Serial.print(" ");
  //Serial.println(encoder0Pos);Serial.print(" ");
  //Serial.println(teach_encoder0Pos);Serial.print(" ");
  //Serial.println(distanceSensor.measureDistanceCm());
  Serial.print(BUTTON_STOP_STATE);
  Serial.println(encoder0Pos);
  Serial.println(teach_encoder0Pos);
}

void light(){
  delay(100);
  if (RELAY_LIGHT_FLAG==1){
    analogWrite(RELAY_LIGHT,100);
    RELAY_LIGHT_FLAG=2;}
  else if (RELAY_LIGHT_FLAG==2){
    digitalWrite(RELAY_LIGHT,LOW);
    RELAY_LIGHT_FLAG=0;}
  else if (RELAY_LIGHT_FLAG==0)
    {digitalWrite(RELAY_LIGHT,HIGH);
    RELAY_LIGHT_FLAG=1;}
  while (BUTTONS==LIGHT_B){
    delay(10);
    sense();
  }
  
  Serial.print("RELAY_LIGHT_FLAG");
  Serial.println(RELAY_LIGHT_FLAG);
  //delay(1000);
}

void fan(){
  delay(100);
  if (RELAY_FAN_FLAG==1){digitalWrite(RELAY_FAN_LOW,LOW);
  RELAY_FAN_FLAG=0;}
  else {digitalWrite(RELAY_FAN_LOW,HIGH);
  RELAY_FAN_FLAG=1;}
  while (BUTTON_FAN_STATE==0){
    delay(10);
    sense();
  }
  Serial.println("fan");
}

void loop(){
 acc=0;
  Serial.println("x");
  Serial.println("x");
 analogWrite25k(ENA, 0);
 buttonsFlag=0;
 sense();
 if (BUTTON_STOP_STATE==0) encoder0Pos=20;
 if (BUTTONS==LIGHT_B)light();
 if (BUTTONS==FAN_B)fan();
 if (BUTTONS==GO_B && BUTTON_STOP_STATE==0){Serial.println("go to set position");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  goTo();
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (BUTTONS==goHome_B && BUTTON_STOP_STATE==1){Serial.println("go home");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  goHome();
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (BUTTONS==goUp_B && BUTTON_STOP_STATE==1){Serial.println("go up");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  while (BUTTONS==goUp_B && BUTTON_STOP_STATE==1){goUp(300,100);sense();}
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (BUTTONS==goDown_B){Serial.println("go down");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  while (BUTTONS==TEACH_B){goDown(300,100);sense();}
  digitalWrite(RELAY_BREAK,HIGH);
 }


 if (BUTTONS==TEACH_B && BUTTON_STOP_STATE==0){Serial.println("teach");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  encoder0Pos=0;
  teach();
  digitalWrite(RELAY_BREAK,HIGH);
 }
//Serial.println("loop");
}
