#ifndef INI_h
#define INI_h
volatile int ticks = 0;
volatile int last_Pos = 0;
int last_vel = 0;
int cur_vel = 0;

byte BUTTONS = BUTTONS_INI;

//STATE
//int  BUTTON_GO_STATE=1;
//int  BUTTON_goUp_STATE=1;
//int  BUTTON_goDown_STATE=1;
//int  BUTTON_TEACH_STATE=1;
//int  BUTTON_FAN_STATE=1;
//int  BUTTON_LIGHT_STATE=1;
//int  BUTTON_STOP_STATE=1;
int  AMPS_STATE=1;

// States
States state;
bool enteringState;

//flags
bool RELAY_FAN_FLAG=1;
bool RELAY_LIGHT_FLAG=0;
bool UP_FLAG;
bool DOWN_FLAG;
bool NEAR_FLAG=0;

//variables
int x=10;
int acc=3;
unsigned long setPoint;//DISTANCE LEARNED FROM TEACH
//int GO_DOWN_FLAG=0;
int teach_encoder0Pos=0;
int dly=200;
int buttonsFlag=0;
long last_time=0;
long cur_time=0;
long last_pos=0;
long cur_pos=0;
int PWM=0;
int retract_pos=0;
//

#endif
