//SHADE CRYO CONFINMENT BY BOB MCGRATH 9/1/2019
//MECHANICAL CONTROLE FOR A HAUNT IN THE FIELD OF SCREAMS

#include <ArduinoRS485.h> // the ArduinoDMX library depends on ArduinoRS485
#include <ArduinoDMX.h>
const int universeSize = 16;

// constants won't change. They're used here to set pin numbers:
//io 0 and 1 are for DMX

//delys for show.  this is all that should be edited.  delays are in milliseonds.
const int room1delay(1);//delay until doors open
const int room2delay(30000);//delay between room start and cryo chambers spin
const int room2delay2(5000);//delay after cryo chamber rotate before the doors open
const int room3delay(15000);//delay until the celing scare starts

const int st = 25; //sensor threshold

const int cryoAsense1 =  A5; //Hall sensor   
const int cryoAsense2 =  A4; //Hall sensor
const int cryoAenable =  4;//relay

const int cryoBsense1 =  A3; //Hall sensor   
const int cryoBsense2 =  A2;//Hall sensor
const int cryoBenable =  2;//relay

const int tsSense1 = A1;//Hall sensor
const int tsSense2 = A0;//Hall sensor
const int tsDirection =  9;//relay
const int tsEnable =  8 ;//relay

const int showStart1 = 11; // vehicle sensor
const int showStart2 = 12; // button
const int showStart3 = 13; // button

const int garage1 = 3;//relays for garage doors
const int garage2 = 7;
const int garage3 = 6;

const int peppersGhost = 5;//digital out


// variables will change:
int cryoAsense1state = 1;
int cryoAsense2state = 1;// variable for reading the sensor status
int cryoAsense1stateLast = 1;//variables for debouncing
int cryoAsense2stateLast = 1;

int cryoBsense1state = 1;
int cryoBsense2state = 1;
int cryoBsense1stateLast = 1;
int cryoBsense2stateLast = 1;

int tsSense1state = 1;
int tsSense2state = 1;
int tsSense1stateLast =  1;
int tsSense2stateLast =  1;

int showStart1state = 1; 
int showStart2state = 1; 
int showStart3state = 1; 

int show1flag = 1;
int show2flag = 1;
int show3flag = 0;//the default state is to start up with room 3 end available in order to reset the gantry if it gets stuck.  
int showEndFlag = 1;

int channel = 1;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the DMX library with the universe size
  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1);} // wait for ever

  pinMode(cryoAenable, OUTPUT);
  pinMode(cryoBenable, OUTPUT);
  pinMode(tsDirection, OUTPUT);
  pinMode(tsEnable, OUTPUT);

  pinMode(garage1, OUTPUT);
  pinMode(garage2, OUTPUT);
  pinMode(garage3, OUTPUT);
  pinMode(peppersGhost, OUTPUT);
  
  pinMode(cryoAsense1, INPUT_PULLUP);
  pinMode(cryoAsense2, INPUT_PULLUP);
  pinMode(cryoBsense1, INPUT_PULLUP);
  pinMode(cryoBsense2, INPUT_PULLUP);
  pinMode(tsSense1, INPUT_PULLUP);
  pinMode(tsSense2, INPUT_PULLUP);
  
  pinMode(showStart1, INPUT_PULLUP);
  pinMode(showStart2, INPUT_PULLUP);
  pinMode(showStart3, INPUT_PULLUP);
  
  digitalWrite(cryoAenable, LOW);
  digitalWrite(cryoBenable, LOW);
  digitalWrite(tsDirection, LOW);
  digitalWrite(tsEnable, LOW);
  
  digitalWrite(garage1, LOW);
  digitalWrite(garage2, LOW);
  digitalWrite(garage3, LOW);
  digitalWrite(peppersGhost, LOW);

  DMX.beginTransmission();
  DMX.write(1, 0);
  DMX.write(2, 0);
  DMX.write(3, 0);
  DMX.write(4, 0);
  DMX.endTransmission();
}

int DMXgo() {
  //"channel" is passed befor entering this function to decide which dmx channel will be sent.  
  // set channel 1 value to 255
  DMX.beginTransmission();
  DMX.write(channel, 255);
  DMX.endTransmission();

  delay(1000);

  // set channel 1 value to 0
  DMX.beginTransmission();
  DMX.write(channel, 0);
  DMX.endTransmission();

  //delay(700);
}


void sense() {
  //delay(2);
  //Serial.println(cryoAsense1state);
  //Serial.print(cryoAsense2state);
  cryoAsense1stateLast = cryoAsense1state;
  cryoAsense2stateLast = cryoAsense2state;
  cryoAsense1state = analogRead(cryoAsense1);
   //delay(2);
  cryoAsense2state = analogRead(cryoAsense2);
  //Serial.print(cryoBsense1state);
  //Serial.print(cryoBsense2state);
  cryoBsense1stateLast = cryoBsense1state;
  cryoBsense2stateLast = cryoBsense2state;
  cryoBsense1state = analogRead(cryoBsense1);
   //delay(2);
  cryoBsense2state = analogRead(cryoBsense2);
   //delay(2);
  //Serial.print(tsSense1state);
  //Serial.print(tsSense2state);
  tsSense1stateLast = tsSense1state;
  tsSense2stateLast = tsSense2state;
  tsSense1state = digitalRead(tsSense1);
  tsSense2state = digitalRead(tsSense2);
  showStart1state = digitalRead(showStart1); 
  showStart2state = digitalRead(showStart2); 
  showStart3state = digitalRead(showStart3);
}

void reset(){
  //delay(5);
  cryoAsense1stateLast = 1000;
  cryoAsense2stateLast = 1000;
  cryoAsense1state = 1000;
  cryoAsense2state = 1000;
  
  cryoBsense1stateLast = 1000;
  cryoBsense2stateLast = 1000;
  cryoBsense1state = 1000;
  cryoBsense2state = 1000;
  
  tsSense1stateLast = 1;
  tsSense2stateLast = 1;
  tsSense1state = 1;
  tsSense2state = 1;
}

void cryoGo(){
  //this loop starts both chambers and then waits for the B to stop and then A
    //Serial.println("A");  
    digitalWrite(cryoAenable, HIGH);
    delay(100); 
    digitalWrite(cryoBenable, HIGH);
reset();
    delay(3000);//startup delay for chambers
    while ((cryoBsense2state > st || cryoBsense2stateLast > st))sense();
    //Serial.println("B");
    sense();
    sense();
    digitalWrite(cryoBenable, LOW);
reset();
    while ((cryoAsense1state > st && cryoAsense1stateLast > st))sense();
    //Serial.println("C");
    digitalWrite(cryoAenable, LOW);
    delay(500);
  }
   
void cryoGoBack(){
  //this is the reset loop.  each chamber spins one at a time
    //Serial.println("W");
    digitalWrite(cryoAenable, HIGH);
reset();
delay(2000);
    while (cryoAsense2state > st || cryoAsense2stateLast > st)sense();
    //Serial.println("X");
    digitalWrite(cryoAenable, LOW);
reset();
    //digitalWrite(cryoBdirection, LOW);    
    digitalWrite(cryoBenable, HIGH);
    while ((cryoBsense1state > st || cryoBsense1stateLast > st))sense();
    //Serial.println("Y");
    digitalWrite(cryoBenable, LOW);
reset();
  }

void tsGo(){
  //this is the touch scare loop
  digitalWrite(tsEnable, HIGH);
  digitalWrite(tsDirection, HIGH);
reset();
  while (tsSense1state == HIGH || tsSense1stateLast == HIGH)sense();
  digitalWrite(tsEnable, LOW);
}
void tsGoBack(){
  //this is the touch scare return loop
  digitalWrite(tsEnable, HIGH);
  digitalWrite(tsDirection, LOW);
reset();
delay(5000);
  while (tsSense2state == HIGH || tsSense2stateLast == HIGH)sense();
  digitalWrite(tsEnable, LOW);
}

void show1(){
  channel=1;
  DMXgo();//sendDMX
  delay(room1delay);//variable delay based on show timing
  digitalWrite(garage1, HIGH);
  digitalWrite(garage2, HIGH);
  delay(700);
  digitalWrite(garage1, LOW);
  digitalWrite(garage2, LOW);
  show1flag = 0;
  show2flag = 1;
  show3flag = 0;
  showEndFlag = 0; 
}

void show2(){
  channel=2;
  DMXgo();//sendDMX
  //delay(4000);
  digitalWrite(peppersGhost, HIGH);
  delay(700);
  digitalWrite(peppersGhost, LOW);
  delay(room2delay);//variable delay based on show timing
  cryoGo();
  delay(room2delay2);//variable delay based on show timing
  DMXgo();//sendDMX??
  digitalWrite(garage2, HIGH);
  digitalWrite(garage3, HIGH);
  delay(1000);  
  digitalWrite(garage2, LOW);
  digitalWrite(garage3, LOW);
  show1flag = 0;
  show2flag = 0;
  show3flag = 1;
  showEndFlag = 0;
}

void show3(){
  channel=3;
  DMXgo();//sendDMX
  delay(room3delay);//variable delay based on show timing
  tsGo();
  show1flag = 0;
  show2flag = 0;
  show3flag = 0;
  showEndFlag = 1;
  
}

void showEnd(){
  digitalWrite(garage3, HIGH);
  delay(1000);  
  digitalWrite(garage3, LOW);
  channel=4;
  DMXgo();//sendDMX
  cryoGoBack();
  tsGoBack();  
  show1flag = 1;
  show2flag = 0;
  show3flag = 0;
  showEndFlag = 0; 
}
  
void loop() {
  //this is the main loop.  shows start if they are allowed to.  all shows can start on powerup then they must run in order.  
  sense();
  if (showStart1state==0 && show1flag==1) show1();
  if (showStart2state==0 && show2flag==1) show2();
  if (showStart3state==0 && show3flag==1) show3();
  if (showStart3state==0 && showEndFlag==1) showEnd();

}
