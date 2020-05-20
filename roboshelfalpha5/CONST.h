#ifndef CONST_h
#define CONST_h

//#include "Arduino.h"
//#include "PINS.h"

//motor drive pins
const int ENA=9;
const int IN1=7;
const int IN2=6;
const int encoder0PinA = 2;

//buttons
const byte BUTTONS_INI=0b00111000;
const int BUTTON_STOP = A1;
const int BUTTON_SPAGHETTI = A4;
const int BUTTON_FINGER = A5;
const byte LIGHT_B=0b00111100;
const byte FAN_B=0b00111010;
const byte GO_B=0b00111001;
const byte goHome_B=0b00111001;
const byte goUp_B=0b00011000;
const byte goDown_B=0b00101000;
const byte TEACH_B=0b00110000;

//relays
const int RELAY_FAN_HIGH=A2;
const int RELAY_FAN_LOW=A3;
const int RELAY_LIGHT=3;
const int RELAY_BREAK=5;

//current sensor
//int AMP_SENSE=A0;
const int AMPS=A0;

#endif
