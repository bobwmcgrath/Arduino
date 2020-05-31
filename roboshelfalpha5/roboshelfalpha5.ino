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
 void setState(WAIT);
}

void doEncoder() //interupt based encoder ticks
{
  if (GO_DOWN_FLAG==1) {
    ticks++;
  } else {
    ticks--;
  }
}

void loop(){
 owb::buttons(BUTTONS);
 last_time=cur_time;
 cur_time=millis();
 last_pos=cur_pos;
 cur_pos=sense::ticks2in(ticks);
 cur_vel=sense::velocitize(last_time, cur_time, last_pos, cur_pos);
 pins::analogWrite25k(ENA,PWM);

 if (BUTTONS==LIGHT_B)light_fan::light(RELAY_LIGHT_FLAG);
 if (BUTTONS==FAN_B)light_fan::fan(RELAY_FAN_FLAG);


 switch (state) {

    case WAIT: 
    if (enteringState)
    {
      Serial.println("WAIT");
      enteringState = false;
      digitalWrite(RELAY_BREAK,HIGH);
    }
        PWM=0
        if (BUTTONS==GO_PRESET_B && digitalRead(BUTTON_STOP)==0)setState(GO_PRESET);
        if (BUTTONS==GO_USER_B && digitalRead(BUTTON_STOP)==0)setState(GO_USER);
        if (BUTTONS==goHome_B && digitalRead(BUTTON_STOP)==1)setState(GO_HOME);
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
    
      PWM = goToPosition(preset_pos, vel, acc, cur_pos, cur_vel, NEAR_FLAG, PWM);
      
      if (digitalRead(BUTTON_SPAGHETTI)==1 && NEAR_FLAG==0)
      {
      setState(SPAGHETTI_SAFETY);
      }
    break;

    case SPAGHETTI_SAFETY: 
    if (enteringState)
    {
      Serial.println("SPAGHETTI_SAFETY");
      enteringState = false;
      retract = cur_pos - 2.0       
    }
    PWM = goToPosition(preset_encoder0Pos, max_vel, max_acc, cur_pos, cur_vel);
    break;
    
}
