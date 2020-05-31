#ifndef motor_h
#define motor_h

#include "Arduino.h"
#include "CONST.h"

namespace motor{

int goToPosition(const long& goal_pos, const long& goal_vel, const long& acc, const long& cur_pos, const long& cur_vel, int& NEAR_FLAG);

}

#endif
