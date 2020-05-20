#ifndef INI_h
#define INI_h

//motor drive pins
int ENA=9;
int IN1=7;
int IN2=6;
int encoder0PinA = 2;
int ULTRASONIC_INTERUPT = 3;

volatile unsigned int encoder0Pos = 0;

//buttons
const byte BUTTONS_INI=0b00111000;
byte BUTTONS = BUTTONS_INI;
int BUTTON_STOP = A1;
int BUTTON_SPAGHETTI = A4;
int BUTTON_FINGER = A5;
const byte LIGHT_B=0b00111100;
const byte FAN_B=0b00111010;
const byte GO_B=0b00111001;
const byte goHome_B=0b00111001;
const byte goUp_B=0b00011000;
const byte goDown_B=0b00101000;
const byte TEACH_B=0b00110000;


//STATE
int  BUTTON_GO_STATE=1;
int  BUTTON_goUp_STATE=1;
int  BUTTON_goDown_STATE=1;
int  BUTTON_TEACH_STATE=1;
int  BUTTON_FAN_STATE=1;
int  BUTTON_LIGHT_STATE=1;
int  BUTTON_STOP_STATE=1;
int  AMPS_STATE=1;


//current sensor
//int AMP_SENSE=A0;
int AMPS=A0;

//relays
int RELAY_FAN_HIGH=A2;
int RELAY_FAN_LOW=A3;
int RELAY_LIGHT=3;
int RELAY_BREAK=5;

//variables
int x=10;
bool UP_FLAG;
bool DOWN_FLAG;
int acc=1;
unsigned long setPoint;//DISTANCE LEARNED FROM TEACH
int startTime=0;
int currentTime=0;
int RELAY_FAN_FLAG=1;
int RELAY_LIGHT_FLAG=0;
int GO_DOWN_FLAG=0;
int teach_encoder0Pos=0;
int dly=200;
int buttonsFlag=0;
//

#endif
