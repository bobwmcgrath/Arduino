#include "light_fan.h"



namespace light_fan{

  bool light(bool& RELAY_LIGHT_FLAG){
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
  while (owb::buttons()==LIGHT_B){
    delay(10);
  }
  
  Serial.println("LIGHT");

  return RELAY_LIGHT_FLAG;
}

bool fan(bool& RELAY_FAN_FLAG){
  delay(100);
  
  if (RELAY_FAN_FLAG==1){
    digitalWrite(RELAY_FAN_LOW,LOW);
    RELAY_FAN_FLAG=0;
    }
  else {
    digitalWrite(RELAY_FAN_LOW,HIGH);
    RELAY_FAN_FLAG=1;
    }
    
  while (owb::buttons()==FAN_B){
    delay(10);
    //sense();
  }
  
  Serial.println("fan");
  return RELAY_FAN_FLAG;
}
  
}
