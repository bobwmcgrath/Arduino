#include "sense.h"

namespace sense{

  float velocitize(const int& cur_pos, const int& last_pos, const int& cur_time, const int& last_time)
  {
    return ((cur_pos - last_pos)/(cur_time-last_time));
  }

  float ticks2in(const int& ticks)
  {
    return ticks/ticks_per_inch;
  }
  
}
