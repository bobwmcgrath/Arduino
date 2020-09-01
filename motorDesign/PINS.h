#ifndef PINS_h
#define PINS_h

#include "Arduino.h"
#include "CONST.h"


namespace pins{
 bool GDF();
 void PWMconfig();
 void pinINI();
 void analogWrite25k(int pin, int value);
}

#endif
