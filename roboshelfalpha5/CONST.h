#ifndef CONST_h
#define CONST_h

//#include "Arduino.h"
//#include "PINS.h"
#include "sense.h"

enum States {
  //STARTUP,          // 0
  WAIT,               // 1
  GO_PRESET,          // 2
  GO_USER,            // 3
  GO_HOME,            // 4
  GO_UP,              // 5
  GO_DOWN,            // 6
  TEACH,              // 7
  SPAGHETTI_SAFETY,   // 8
  FINGER_SAFETY,      // 9
};

//motor stuff
const int ENA=9;
const int IN1=7;
const int IN2=6;
const int encoder0PinA = 2;
const int max_PWM=300;
const int min_PWM=-300;

//buttons
const byte BUTTONS_INI=0b11110000;
const int BUTTON_STOP = A1;
const int BUTTON_SPAGHETTI = A4;
const int BUTTON_FINGER = A5;
const byte LIGHT_B=    0b11111000;
const byte FAN_B=      0b11110100;
const byte GO_PRESET_B=0b11110001;
const byte GO_USER_B=  0b11110010;
const byte goUp_B=     0b11010000;
const byte goDown_B=   0b11100000;
const byte TEACH_B=    0b00110000;

//relays
const int RELAY_FAN_HIGH=A2;
const int RELAY_FAN_LOW=A3;
const int RELAY_LIGHT=3;
const int RELAY_BREAK=5;

//current sensor
//int AMP_SENSE=A0;
const int AMPS=A0;

//ticks2in
const int ticks_per_inch=1000;
const int preset_pos=ticks_per_inch * 13;//sense::in2ticks(13.0);
const int vel = ticks_per_inch * 0.2;//sense::in2ticks(2.0);
const int retract=ticks_per_inch * 2;//sense::in2ticks(2.0);
const int min_vel=ticks_per_inch * 0.05;//sense::in2ticks(0.5);
const int vel_tol=ticks_per_inch * 0.01;
const int near_pos= ticks_per_inch * 2;


#endif
