#ifndef light_fan_h
#define light_fan_h

#include "Arduino.h"
#include "CONST.h"
#include "OWB.h"


namespace light_fan{
  
bool light(bool& RELAY_LIGHT_FLAG, byte& BUTTONS);
bool fan(bool& RELAY_FAN_FLAG, byte& BUTTONS);

}

#endif
