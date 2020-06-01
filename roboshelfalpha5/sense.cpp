#include "sense.h"

namespace sense{

  long velocitize(const long& cur_pos, const long& last_pos, const long& cur_time, const long& last_time)
  {
    return ((abs(cur_pos - last_pos) * 300)/abs(cur_time-last_time));
  }

  //constexpr int in2ticks(const float in)
  //{
  //  int x;
  //  x=ticks_per_inch * in;
  //  return x
  //}
  
}
