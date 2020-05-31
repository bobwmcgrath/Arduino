#include "motor.h"

namespace motor{


  int goToPosition(const float& goal_pos, const float& goal_vel, const int& acc, const float& cur_pos, const long& cur_vel, int& NEAR_FLAG, int PWM){
    if (NEAR_FLAG=1)return min_PWM;
    if (goal_pos-cur_pos<6)return PWM - acc;
    if (abs(cur_vel-goal_vel) < 0.5) return PWM; 
    if (cur_vel<goal_vel) PWM += acc;
    if (cur_vel>goal_vel) PWM -= acc;
    
    

      return PWM;

}
