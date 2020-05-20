//PIN INITILIZATION
#include "PINS.h"

namespace pins{
  
void pinINI()
{
 pinMode(ENA,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT); 
 pinMode(encoder0PinA, INPUT_PULLUP);       // turn on pull-up resistor
 pinMode(BUTTON_STOP,INPUT_PULLUP);
 pinMode(AMPS,INPUT);
 pinMode(A5,OUTPUT);
 pinMode (RELAY_FAN_LOW,OUTPUT);
 pinMode (RELAY_FAN_HIGH,OUTPUT);
 pinMode (RELAY_LIGHT,OUTPUT);
 digitalWrite (RELAY_LIGHT,LOW);
 pinMode (RELAY_BREAK,OUTPUT);
 digitalWrite (RELAY_BREAK,HIGH);
 }

 
}
