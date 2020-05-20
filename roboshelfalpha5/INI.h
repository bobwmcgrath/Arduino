#ifndef INI_h
#define INI_h
volatile unsigned int encoder0Pos = 0;

byte BUTTONS = BUTTONS_INI;

//STATE
int  BUTTON_GO_STATE=1;
int  BUTTON_goUp_STATE=1;
int  BUTTON_goDown_STATE=1;
int  BUTTON_TEACH_STATE=1;
int  BUTTON_FAN_STATE=1;
int  BUTTON_LIGHT_STATE=1;
int  BUTTON_STOP_STATE=1;
int  AMPS_STATE=1;



//variables
const int x=10;
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
const int dly=200;
int buttonsFlag=0;
//

#endif
