#include "motor.h"


namespace motor{


  int goToPosition(const int& goal_pos, const int& goal_vel, const int& acc, const int& cur_pos, const int& cur_vel, int PWM){
    Serial.println("1");
    if (abs(goal_pos-cur_pos)<near_pos && abs(cur_vel-min_vel) < vel_tol)return PWM;
    Serial.println("2"); 
    if (abs(goal_pos-cur_pos)<near_pos && cur_vel<min_vel)return PWM+acc;
    Serial.println("3");
    if (abs(goal_pos-cur_pos)<near_pos)return PWM - acc;
    Serial.println("4");
    if (abs(cur_vel-goal_vel) < vel_tol) return PWM; 
    Serial.println("5");
    if (cur_vel<goal_vel) return PWM + acc/2;
    Serial.println("6");
    if (cur_vel>goal_vel) return PWM - acc/2;
    Serial.println("7");
    
    

      return PWM;

  }
}
