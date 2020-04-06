/* roboshelf by Bob*/
#include <EEPROM.h>
#include <OneWire.h>
#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(A4, A5);  // Initialize sensor that uses digital pins 13 and 12.

//OWB stuff
#define DS2408_ONEWIRE_PIN  (8)
#define DS2408_FAMILY_ID    0x29
#define DS2408_ACCESS_READ  0xF5
#define DS2408_ACCESS_WRITE 0x5A
#define DS2408_ACK_SUCCESS  0xAA
#define DS2408_ACK_ERROR    0xFF


//motor drive pins
int ENA=9;
int IN1=7;
int IN2=6;
int encoder0PinA = 2;
int ULTRASONIC_INTERUPT = 3;

volatile unsigned int encoder0Pos = 0;

//buttons
byte BUTTONS = 0b00000000;
int BUTTON_STOP = A1;


//STATE
int  BUTTON_GO_STATE=1;
int  BUTTON_goUp_STATE=1;
int  BUTTON_goDown_STATE=1;
int  BUTTON_TEACH_STATE=1;
int  BUTTON_FAN_STATE=1;
int  BUTTON_LIGHT_STATE=1;
int  BUTTON_STOP_STATE=1;
int  AMPS_STATE=1;
int  LAST_AMPS_STATE=1;

//current sensor

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
//int acc=1;
unsigned long setPoint;//DISTANCE LEARNED FROM TEACH
int startTime=0;
int currentTime=0;
int RELAY_FAN_FLAG=1;
int RELAY_LIGHT_FLAG=0;
int GO_DOWN_FLAG=0;
int teach_encoder0Pos=0;
int dly=200;
int buttonsFlag=0;
int acc=0;
//

OneWire oneWire(DS2408_ONEWIRE_PIN);
uint8_t address[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };



void printBytes(uint8_t* addr, uint8_t count, bool newline=0) 
{
  for (uint8_t i = 0; i < count; i++) 
  {
    Serial.print(addr[i]>>8, HEX);
    Serial.print(addr[i]&0x0f, HEX);
    Serial.print(" ");
  }
  if (newline)
  {
    Serial.println();
  }
}

byte read(void)
{    
  bool ok = false;
  uint8_t results;

  oneWire.reset();
  oneWire.select(address);
  oneWire.write(DS2408_ACCESS_READ);

  results = oneWire.read();                 /* Get the register results   */
  ok = (!results & 0x0F) == (results >> 4); /* Compare nibbles            */
  //results &= 0x0F;                          /* Clear inverted values      */

  oneWire.reset();
  
  // return ok ? results : -1;
  return results;
}

bool write(uint8_t state)
{
  uint8_t ack = 0;
  
  /* Top six bits must '1' */
  state |= 0xFC;
  
  oneWire.reset();
  oneWire.select(address);
  oneWire.write(DS2408_ACCESS_WRITE);
  oneWire.write(state);
  oneWire.write(~state);                    /* Invert data and resend     */    
  ack = oneWire.read();                     /* 0xAA=success, 0xFF=failure */  
  if (ack == DS2408_ACK_SUCCESS)
  {
    oneWire.read();                          /* Read the status byte      */
  }
  oneWire.reset();
    
  return (ack == DS2408_ACK_SUCCESS ? true : false);
}

void setup()
{

 //OWB stuff
  Serial.begin(115200);  
  
  Serial.println(F("Looking for a DS2408 on the bus"));
  
  /* Try to find a device on the bus */
  oneWire.reset_search();
  delay(250);
  if (!oneWire.search(address)) 
  {
    printBytes(address, 8);
    Serial.println(F("No device found on the bus!"));
    oneWire.reset_search();
    while(1);
  }
  
  /* Check the CRC in the device address */
  if (OneWire::crc8(address, 7) != address[7]) 
  {
    Serial.println(F("Invalid CRC!"));
    while(1);
  }
  
  /* Make sure we have a DS2408 */
  if (address[0] != DS2408_FAMILY_ID) 
  {
    printBytes(address, 8);
    Serial.println(F(" is not a DS2413!"));
    while(1);
  }
  
  Serial.print(F("Found a DS2408: "));
  printBytes(address, 8);
  Serial.println(F(""));
//end of OWB stuff


//read eeprom for set distance
 EEPROM.get(0,teach_encoder0Pos);
  
// Configure Timer 1 for PWM @ 25 kHz.
    TCCR1A = 0;           // undo the configuration done by...
    TCCR1B = 0;           // ...the Arduino core library
    TCNT1  = 0;           // reset timer
    TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
           | _BV(COM1B1)  // same on ch; B
           | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
    TCCR1B = _BV(WGM13)   // ditto
           | _BV(CS10);   // prescaler = 1
    ICR1   = 320;         // TOP = 320

//PIN INITILIZATION
 pinMode(ENA,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT); 
 pinMode(encoder0PinA, INPUT_PULLUP);       // turn on pull-up resistor
 pinMode(US_INTERUPT, INPUT_PULLUP);       // turn on pull-up resistor
 attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
 attachInterrupt(1, USlisten, CHANGE);  // encoder pin on interrupt 1 - pin 3
 pinMode(BUTTON_STOP,INPUT_PULLUP);
 pinMode(AMPS,INPUT);
 pinMode(A5,OUTPUT);
 pinMode (RELAY_FAN_LOW,OUTPUT);
 pinMode (RELAY_FAN_HIGH,OUTPUT);
 pinMode (RELAY_LIGHT,OUTPUT);
 digitalWrite (RELAY_LIGHT,LOW);
 pinMode (RELAY_BREAK,OUTPUT);
 digitalWrite (RELAY_BREAK,HIGH);

 

 // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
}
 sense();

Serial.println("setup");
//encoder0Pos=0;
}

//AN ANALOG WRITE THAT PWMS AT FASTER THAN THE NORMAL 490HZ
void analogWrite25k(int pin, int value)
{
    switch (pin) {
        case 9:
            OCR1A = value;
            break;
        default:
            // no other pin will work
            break;
    }
}

void doEncoder() {
  
  if (GO_DOWN_FLAG==1) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }

  //Serial.println (encoder0Pos, DEC);
}

void US_listen(){
  US_STATE=digitalRead(US_interupt);
  if (US_STATE==1){
    US_START=micros();
  }
  if (US_STATE==0){
    LAST_US_DISTANCE=US_DISTANCE
    US_END=micros();
    US_END-US_START=US_DISTANCE;
  }  
}

void US_send(){
  digitalWrite(US_trigger, HIGH);
  delayMicroseconds(20);
  digitalWrite(US_trigger, LOW); 
}

void goUp(int spd, int x)
{
 GO_DOWN_FLAG=0;
 //sense();
 //if (BUTTON_STOP_STATE == 0){   
 digitalWrite(IN1,LOW);// rotate forward
 digitalWrite(IN2,HIGH);
 analogWrite25k(ENA, spd);// motor speed  
 //Serial.println("up");
 delay(x);
  
 //}
} 

void goDown(int spd, int x)
{ 
 US_send();
 GO_DOWN_FLAG=1;  
 digitalWrite(IN1,HIGH);// rotate reverse
 digitalWrite(IN2,LOW);
 analogWrite25k(ENA, spd);// motor speed  
 delay(x);
 if US_DISTANCE-LAST_US_DISTANCE>
 //Serial.println("down");
 //Serial.println(spd);
} 

void goTo(){

while (teach_encoder0Pos>encoder0Pos){
if (acc<300&&teach_encoder0Pos-encoder0Pos>100)acc+=5;
else if (acc>50)acc-=5;
if (BUTTONS==0b00000000) buttonsFlag=1;
if (BUTTONS==0b00000001&&buttonsFlag==1){
  while (BUTTONS==0b00000001){
    delay(10);
    sense();
    }
  return 0; 
}
goDown(acc,10);
sense();}
  
}

void goHome(){
  while (BUTTON_STOP_STATE==1){
  if (acc<300&&encoder0Pos>100)acc+=5;
  else if (acc>50)acc-=5;
  if (BUTTONS==0b00000000) buttonsFlag=1;
  if (BUTTONS==0b00000001&&buttonsFlag==1){
    analogWrite25k(ENA, 0);
    while (BUTTONS==0b00000001){
      delay(10);
      sense();
    }
    return 0;
  }
  sense();
  goUp(acc,10);
  //Serial.println("home");
  }
}

void teach(){ 
 delay(10);
 while (BUTTONS==0b00001000){
    goDown(acc,100);
    sense();
    if (acc<300)acc+=10;
    }
    goUp(0,100);
    delay(dly);
  teach_encoder0Pos=encoder0Pos;
  EEPROM.put(0,teach_encoder0Pos);
}

void sense(){
  delay(10);
    BUTTONS = read();
  if (BUTTONS == -1)
    Serial.println(F("Failed reading the DS2408"));
  else
    //Serial.println(BUTTONS, BIN);
  delay(10);
  BUTTON_STOP_STATE=digitalRead(BUTTON_STOP);
  LAST_AMPS_STATE=AMPS_STATE;
  AMPS_STATE=analogRead(A0);
  Serial.println(BUTTONS,BIN);Serial.print(" ");
  //Serial.println(encoder0Pos);Serial.print(" ");
  //Serial.println(teach_encoder0Pos);Serial.print(" ");
  Serial.println(distanceSensor.measureDistanceCm());
}

int light(){
  delay(100);
  if (RELAY_LIGHT_FLAG==1){
    analogWrite(RELAY_LIGHT,100);
    RELAY_LIGHT_FLAG=2;}
  else if (RELAY_LIGHT_FLAG==2){
    digitalWrite(RELAY_LIGHT,LOW);
    RELAY_LIGHT_FLAG=0;}
  else if (RELAY_LIGHT_FLAG==0)
    {digitalWrite(RELAY_LIGHT,HIGH);
    RELAY_LIGHT_FLAG=1;}
  while (BUTTONS==0b00000100){
    delay(10);
    sense();
  }
  
  Serial.print("RELAY_LIGHT_FLAG");
  Serial.println(RELAY_LIGHT_FLAG);
  //delay(1000);
}

void fan(){
  delay(100);
  if (RELAY_FAN_FLAG==1){digitalWrite(RELAY_FAN_LOW,LOW);
  RELAY_FAN_FLAG=0;}
  else {digitalWrite(RELAY_FAN_LOW,HIGH);
  RELAY_FAN_FLAG=1;}
  while (BUTTON_FAN_STATE==0){
    delay(10);
    sense();
  }
  Serial.println("fan");
}

void loop(){
 //delay(dly);
 //Serial.println(distanceSensor.measureDistanceCm());
 acc=0;
 //Serial.println("x");
 analogWrite25k(ENA, 0);
 buttonsFlag=0;
 sense();
 if (BUTTON_STOP_STATE==0) encoder0Pos=20;
 if (BUTTONS==0b00000100)light();
 if (BUTTONS==0b00000010)fan();
 if (BUTTONS==0b00000001 && BUTTON_STOP_STATE==0){Serial.println("go to set position");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  goTo();
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (BUTTONS==0b00000001 && BUTTON_STOP_STATE==1){Serial.println("go home");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  goHome();
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (BUTTONS==0b00100000 && BUTTON_STOP_STATE==1){Serial.println("go up");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  while (BUTTONS==0b00100000 && BUTTON_STOP_STATE==1){goUp(acc,100);
  if (acc<300)acc+=10;
  sense();}
  goUp(0,100);
  delay(dly);
  digitalWrite(RELAY_BREAK,HIGH);
 }
 if (BUTTONS==0b00010000){Serial.println("go down");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  while (BUTTONS==0b00010000){
    goDown(acc,100);
    sense();
    if (acc<300)acc+=10;
    }
    goUp(0,100);
    delay(dly);
  digitalWrite(RELAY_BREAK,HIGH);
 }


 if (BUTTONS==0b00001000 && BUTTON_STOP_STATE==0){Serial.println("teach");
  digitalWrite(RELAY_BREAK,LOW);
  delay(dly);
  encoder0Pos=0;
  teach();
  digitalWrite(RELAY_BREAK,HIGH);
 }
//Serial.println("loop");
}
