#include <ArduinoRS485.h> // the ArduinoDMX library depends on ArduinoRS485
#include <ArduinoDMX.h>
const int universeSize = 16;

// constants won't change. They're used here to set pin numbers:
//io 2 and 3 are for DMX soft serial

const int cryoAsense1 =  11; //Hall sensor   
const int cryoAsense2 =  10; //Hall sensor
const int cryoAdirection =  5;//relay
const int cryoAenable =  4;//relay

const int cryoBsense1 =  13; //Hall sensor   
const int cryoBsense2 =  12;//Hall sensor
const int cryoBdirection =  9;//relay
const int cryoBenable =  8;//relay

const int tsSense1 = A1;//Hall sensor
const int tsSense2 = A0;//Hall sensor
const int tsDirection =  3;//relay
const int tsEnable =  2 ;//relay

const int showStart1 = A5; // vehicle sensor
const int showStart2 = A4; // button
const int showStart3 = A3
; // button

const int garage12 = 6;//relay
const int garage23 = 7;//relay

const int peppersGhost = A2;//digital out


// variables will change:
int cryoAsense1state = 1;
int cryoAsense2state = 1;// variable for reading the sensor status
int cryoAsense1stateLast = 1;//variables for d ebouncing
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

int goFlag = 1;
int goBackFlag = 1;

void setup() {
  Serial.begin(9600);
  /*while (!Serial);

  // initialize the DMX library with the universe size
  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1);} // wait for ever
  */  
  //pinMode(cryoAdirection, OUTPUT);
  pinMode(cryoAenable, OUTPUT);
  //pinMode(cryoBdirection, OUTPUT);
  pinMode(cryoBenable, OUTPUT);
  pinMode(tsDirection, OUTPUT);
  pinMode(tsEnable, OUTPUT);

  pinMode(garage12, OUTPUT);
  pinMode(garage23, OUTPUT);
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
  digitalWrite(cryoAdirection, LOW);
  digitalWrite(cryoBenable, LOW);
  digitalWrite(cryoBdirection, LOW);
  digitalWrite(tsDirection, HIGH);
  digitalWrite(tsEnable, LOW);
  
  digitalWrite(garage12, LOW);
  digitalWrite(garage23, LOW);
  digitalWrite(peppersGhost, LOW);
}

void DMXgo() {
  // set channel 1 value to 255
  DMX.beginTransmission();
  DMX.write(1, 255);
  DMX.endTransmission();

  delay(700);

  // set channel 1 value to 0
  DMX.beginTransmission();
  DMX.write(1, 0);
  DMX.endTransmission();

  delay(700);
}


void sense() {
  delay(10);
  Serial.println(cryoAsense1state);
  Serial.print(cryoAsense2state);
  cryoAsense1stateLast = cryoAsense1state;
  cryoAsense2stateLast = cryoAsense2state;
  cryoAsense1state = digitalRead(cryoAsense1);
  cryoAsense2state = digitalRead(cryoAsense2);
  //Serial.print(cryoBsense1state);
  //Serial.print(cryoBsense2state);
  cryoBsense1stateLast = cryoBsense1state;
  cryoBsense2stateLast = cryoBsense2state;
  cryoBsense1state = digitalRead(cryoBsense1);
  cryoBsense2state = digitalRead(cryoBsense2);
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
  delay(5);
  cryoAsense1stateLast = 1;
  cryoAsense2stateLast = 1;
  cryoAsense1state = 1;
  cryoAsense2state = 1;
  
  cryoBsense1stateLast = 1;
  cryoBsense2stateLast = 1;
  cryoBsense1state = 1;
  cryoBsense2state = 1;
  
  tsSense1stateLast = 1;
  tsSense2stateLast = 1;
  tsSense1state = 1;
  tsSense2state = 1;
}

void cryoGo(){
    Serial.println("A");
    //digitalWrite(cryoAdirection, LOW);
    //digitalWrite(cryoBdirection, LOW);    
    digitalWrite(cryoAenable, HIGH);
    delay(100);
    
    digitalWrite(cryoBenable, HIGH);
reset();
delay(1000);
    while ((cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH) &&  (cryoBsense1state == HIGH || cryoBsense1stateLast == HIGH))sense();
    Serial.println("B");
    if (cryoAsense1state == LOW || cryoAsense1stateLast == LOW){
    digitalWrite(cryoAenable, LOW);
reset();
    while ((cryoBsense1state == HIGH || cryoBsense1stateLast == HIGH))sense();
    Serial.println("C");
    digitalWrite(cryoBenable, LOW);
    }
    else if (cryoBsense1state == LOW || cryoBsense1stateLast == LOW){
    digitalWrite(cryoBenable, LOW);
reset();
    while ((cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH))sense();
    Serial.println("D");
    digitalWrite(cryoAenable, LOW);
    }
    digitalWrite(cryoAenable, LOW);
    digitalWrite(cryoBenable, LOW);
    delay(500);
    goFlag=0;
    goBackFlag=1;
  }
   
void cryoGoBack(){
    Serial.println("W");
    //digitalWrite(cryoAdirection, LOW);
    digitalWrite(cryoAenable, HIGH);
reset();
delay(2000);
    while (cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH)sense();
    Serial.println("X");
    digitalWrite(cryoAenable, LOW);
reset();
    //digitalWrite(cryoBdirection, LOW);    
    digitalWrite(cryoBenable, HIGH);
    while ((cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    Serial.println("Y");
    digitalWrite(cryoBenable, LOW);
reset();
        goFlag=1;
    goBackFlag=0;
  }

void tsGo(){
  digitalWrite(tsEnable, HIGH);
  digitalWrite(tsDirection, HIGH);
reset();
  while (tsSense1state == HIGH || tsSense1stateLast == HIGH)sense();
  digitalWrite(tsEnable, LOW);
}
void tsGoBack(){
  digitalWrite(tsEnable, HIGH);
  digitalWrite(tsDirection, LOW);
reset();
delay(5000);
  while (tsSense2state == HIGH || tsSense2stateLast == HIGH)sense();
  digitalWrite(tsEnable, LOW);
}

void show1(){
  digitalWrite(garage12, HIGH);
  //DMXgo();//sendDMX
  delay(700);
  digitalWrite(garage12, LOW);
  
}

void show2(){
  //DMXgo();//sendDMX
  //delay(4000);
  digitalWrite(peppersGhost, HIGH);
  delay(700);
  digitalWrite(peppersGhost, LOW);
  tsGoBack();
  //delay(4000);
  cryoGo();
  //DMXgo();//sendDMX??
  digitalWrite(garage23, HIGH);
  delay(700);  
  digitalWrite(garage23, LOW);
  
}

void show3(){
  //DMXgo();//sendDMX
  //delay(7000);
  tsGo();
  cryoGoBack();
  
}
  
void loop() {
  sense();
  if (showStart1state==1) show1();
  if (showStart2state==1) show2();
  if (showStart3state==1) show3();

}
