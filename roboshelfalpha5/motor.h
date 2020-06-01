#ifndef motor_h
#define motor_h

#include "Arduino.h"
#include "CONST.h"
#include "sense.h"

namespace motor{

int goToPosition(const int& goal_pos, const int& goal_vel, const int& acc, const int& cur_pos, const int& cur_vel, int PWM);

}

#endif
