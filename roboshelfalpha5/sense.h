#ifndef sense_h
#define sense_h

#include "Arduino.h"
#include "CONST.h"

namespace sense{

long velocitize(const long& cur_pos, const long& last_pos, const long& cur_time, const long& last_time);
//constexpr int in2ticks(const float in);

}

#endif
