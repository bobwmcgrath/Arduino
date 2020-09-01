//PIN INITILIZATION
#include "PINS.h"
bool GO_DOWN_FLAG=0;

namespace pins{

bool GDF(){
  return GO_DOWN_FLAG;
}

void PWMconfig()
 {
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
 }
 
void pinINI()
 {
 pinMode(ENA,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT); 
 pinMode(encoder0PinA, INPUT_PULLUP);       // turn on pull-up resistor
 pinMode(BUTTON_STOP,INPUT_PULLUP);
 pinMode(BUTTON_SPAGHETTI,INPUT_PULLUP);
 pinMode(BUTTON_FINGER,INPUT_PULLUP);
 pinMode(AMPS,INPUT);
 pinMode(A5,OUTPUT);
 pinMode (RELAY_FAN_LOW,OUTPUT);
 pinMode (RELAY_FAN_HIGH,OUTPUT);
 pinMode (RELAY_LIGHT,OUTPUT);
 digitalWrite (RELAY_LIGHT,LOW);
 pinMode (RELAY_BREAK,OUTPUT);
 digitalWrite (RELAY_BREAK,HIGH);
 }

//AN ANALOG WRITE THAT PWMS AT FASTER THAN THE NORMAL 490HZ
// Output value should be between 0 and 320, inclusive.
void analogWrite25k(int pin, int value)
{   if (value < 0)
    {
      GO_DOWN_FLAG=1;
      digitalWrite(IN1,LOW);// rotate forward
      digitalWrite(IN2,HIGH);
    }

    else if (value > 0)
    {
      GO_DOWN_FLAG=0;
      digitalWrite(IN1,HIGH);// rotate reverse
      digitalWrite(IN2,LOW);
    }
    
    switch (pin) {
        case 9:
            OCR1A = abs(value);
            break;
        default:
            // no other pin will work
            break;
    }
}

}
