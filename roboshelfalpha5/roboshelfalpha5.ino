 /* roboshelf by Bob*/
#include <EEPROM.h>
#include "CONST.h"
#include "INI.h"
#include "OWB.h"
#include "PINS.h"
#include "light_fan.h"
#include "motor.h"
#include "sense.h"




void setState(States newState) {
  enteringState = true;
  state = newState;
}


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
 Serial.println("SETUP");
 setState(WAIT);
}

void doEncoder() //interupt based encoder ticks
{
  if (GO_DOWN_FLAG==0) {
    ticks++;
  } else {
    ticks--;
  }
}

void loop(){
 if (PWM>max_PWM)PWM=max_PWM;
 if (PWM<min_PWM)PWM=min_PWM;
 if (digitalRead(BUTTON_STOP)==0)ticks=0;
 delay(50);
 owb::buttons(BUTTONS);
 last_time=cur_time;
 cur_time=millis();
 last_pos=cur_pos;
 cur_pos=ticks;
 last_vel=cur_vel;
 cur_vel=sense::velocitize(cur_pos, last_pos, cur_time, last_time);
 pins::analogWrite25k(ENA,PWM);
 //Serial.println(vel);
 if (BUTTONS==LIGHT_B)light_fan::light(RELAY_LIGHT_FLAG, BUTTONS);
 if (BUTTONS==FAN_B)light_fan::fan(RELAY_FAN_FLAG, BUTTONS);


 switch (state) {

    case WAIT: 
    if (enteringState)
    {
      Serial.println("WAIT");
      enteringState = false;
      PWM=0;
      delay(dly);
      digitalWrite(RELAY_BREAK,HIGH);
    }
        if (BUTTONS==GO_PRESET_B && digitalRead(BUTTON_STOP)==0)setState(GO_PRESET);
        if (BUTTONS==GO_USER_B && digitalRead(BUTTON_STOP)==0)setState(GO_USER);
        if ((BUTTONS==GO_USER_B || BUTTONS==GO_PRESET_B)&& digitalRead(BUTTON_STOP)==1)setState(GO_HOME);
        if (BUTTONS==goUp_B && digitalRead(BUTTON_STOP)==1)setState(GO_UP);
        if (BUTTONS==goDown_B)setState(GO_DOWN);
        if (BUTTONS==TEACH_B && digitalRead(BUTTON_STOP)==0)setState(TEACH);
      break;
      
    case GO_PRESET: 
    if (enteringState)
    {
      Serial.println("GO_PRESET");
      enteringState = false;
      digitalWrite(RELAY_BREAK,LOW);
      delay(dly);
            
    }
    
      PWM = motor::goToPosition(preset_pos, vel, acc, cur_pos, cur_vel, PWM);
      
      if (digitalRead(BUTTON_SPAGHETTI)==0 && NEAR_FLAG==0){setState(SPAGHETTI_SAFETY);}
      if ((BUTTONS==GO_USER_B || BUTTONS==GO_PRESET_B)&& digitalRead(BUTTON_STOP)==1){
      while (owb::buttons(BUTTONS)==GO_USER_B || owb::buttons(BUTTONS)==GO_PRESET_B) delay(10);
      setState(WAIT);}      
      if (cur_pos>preset_pos){setState(WAIT);}
    break;

    case GO_HOME: 
    if (enteringState)
    {
      Serial.println("GO_HOME");
      enteringState = false;
      digitalWrite(RELAY_BREAK,LOW);
      delay(dly);
            
    }
    
      PWM = motor::goToPosition(0, vel, acc, cur_pos, cur_vel, PWM);
      
      if (digitalRead(BUTTON_SPAGHETTI)==0 && NEAR_FLAG==0)
      {setState(SPAGHETTI_SAFETY);}      
      if (digitalRead(BUTTON_STOP)==0){setState(WAIT);}
    break;

    case SPAGHETTI_SAFETY: 
    if (enteringState)
    {
      Serial.println("SPAGHETTI_SAFETY");
      enteringState = false;
      retract_pos = cur_pos - retract;//sense::in2ticks(2.0);      
    }
    PWM = motor::goToPosition(retract_pos, vel, acc, cur_pos, cur_vel, PWM);
    break;
    
  }
}
