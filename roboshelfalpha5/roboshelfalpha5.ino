 /* roboshelf by Bob*/
#include <EEPROM.h>
//#include <OneWire.h>
#include "CONST.h"
#include "OWB.h"
#include "PINS.h"
#include "INI.h"
#include "light_fan.h"

void setup()
{
 Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
// } 

 owb::OWB_setup();
 pins::PWMconfig();
 pins::pinINI();
 attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
 EEPROM.get(0,teach_encoder0Pos);
 sense();
 Serial.println("setup");
}

void doEncoder() //interupt based encoder ticks
{
  if (GO_DOWN_FLAG==1) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
}



void goUp(int spd, int x)
{
 GO_DOWN_FLAG=0;
 //sense();
 //if (BUTTON_STOP_STATE == 0){   
 digitalWrite(IN1,LOW);// rotate forward
 digitalWrite(IN2,HIGH);
 pins::analogWrite25k(ENA, spd);// motor speed  
 //Serial.println("up");
 delay(x);
  
 //}
} 

void goDown(int spd, int x)
{ 
 GO_DOWN_FLAG=1;  
 digitalWrite(IN1,HIGH);// rotate reverse
 digitalWrite(IN2,LOW);
 pins::analogWrite25k(ENA, spd);// motor speed  
 delay(x);
// if US_DISTANCE-LAST_US_DISTANCE>
 //Serial.println("down");
 //Serial.println(spd);
} 

void goTo(){

while (teach_encoder0Pos>encoder0Pos){
  if (cur_vel<300&&encoder0Pos>1000)acc+=5;
  else if (cur_vel>300&&encoder0Pos>1000)acc-=5;
  else if (cur_vel>50)acc-=5;
if (owb::buttons()==BUTTONS_INI) buttonsFlag=1;
if (owb::buttons()==GO_B&&buttonsFlag==1){
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
  while (digitalRead(BUTTON_STOP)==1){
  if (cur_vel<300&&encoder0Pos>1000)acc+=5;
  else if (cur_vel>300&&encoder0Pos>1000)acc-=5;
  else if (cur_vel>50)acc-=5;
  if (owb::buttons()==goHome_B) buttonsFlag=1;
  if (owb::buttons()==goHome_B&&buttonsFlag==1){
    pins::analogWrite25k(ENA, 0);
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
    if (cur_vel<300)acc+=10;
    if (cur_vel>300)acc-=10;
    }
    goDown(0,100);
    delay(dly);
  teach_encoder0Pos=encoder0Pos;
  EEPROM.put(0,teach_encoder0Pos);
}

float velocitize(){
   return ((encoder0Pos - lastencoder0Pos)/(millis()-lastTime));
}

void sense(){
  cur_vel=velocitize();
  delay(10);
  delay(10);
  //BUTTON_STOP_STATE=digitalRead(BUTTON_STOP);
  AMPS_STATE=analogRead(A0);
  Serial.println(BUTTONS,BIN);//Serial.print(" ");
  //Serial.println(encoder0Pos);Serial.print(" ");
  //Serial.println(teach_encoder0Pos);Serial.print(" ");
  //Serial.println(distanceSensor.measureDistanceCm());
  Serial.print(digitalRead(BUTTON_STOP));
  Serial.println(encoder0Pos);
  Serial.println(teach_encoder0Pos);
}



void loop(){
 acc=0;
 lastTime=millis();
 pins::analogWrite25k(ENA, 0);
 buttonsFlag=0;
 sense();
 if (digitalRead(BUTTON_STOP)==0) encoder0Pos=20;
 if (owb::buttons()==LIGHT_B)light_fan::light(RELAY_LIGHT_FLAG);
 if (owb::buttons()==FAN_B)light_fan::fan(RELAY_FAN_FLAG);
 if (owb::buttons()==GO_B && digitalRead(BUTTON_STOP)==0){Serial.println("go to set position");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  goTo();
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (owb::buttons()==goHome_B && digitalRead(BUTTON_STOP)==1){Serial.println("go home");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  goHome();
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (owb::buttons()==goUp_B && digitalRead(BUTTON_STOP)==1){Serial.println("go up");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  while (owb::buttons()==goUp_B && digitalRead(BUTTON_STOP)==1){goUp(300,100);sense();}
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (owb::buttons()==goDown_B){Serial.println("go down");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  while (owb::buttons()==goDown_B){goDown(300,100);sense();}
  digitalWrite(RELAY_BREAK,HIGH);
 }


 if (owb::buttons()==TEACH_B && digitalRead(BUTTON_STOP)==0){Serial.println("teach");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  encoder0Pos=0;
  teach();
  digitalWrite(RELAY_BREAK,HIGH);
 }
}
