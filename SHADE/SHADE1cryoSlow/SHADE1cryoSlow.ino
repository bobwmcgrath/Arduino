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
const int tsDirection =  1;//relay
const int tsEnable =  0;//relay

const int showStart1 = A6; // vehicle sensor
const int showStart2 = A5; // button
const int showStart3 = A4; // button

const int garage12 = 6;//relay
const int garage23 = 7;//relay

const int peppersGhost = A2;//digital out


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

int goFlag = 1;
int goBackFlag = 1;

void setup() {
  Serial.begin(9600);
  pinMode(cryoAdirection, OUTPUT);
  pinMode(cryoAenable, OUTPUT);
  pinMode(cryoBdirection, OUTPUT);
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
  digitalWrite(cryoAdirection, HIGH);
  digitalWrite(cryoBenable, LOW);
  digitalWrite(cryoBdirection, HIGH);
  digitalWrite(tsDirection, HIGH);
  digitalWrite(tsEnable, LOW);
  
  digitalWrite(garage12, LOW);
  digitalWrite(garage23, LOW);
  digitalWrite(peppersGhost, LOW);
}


void sense() {
  delay(5);
  Serial.println(cryoAsense1state);
  Serial.print(cryoAsense2state);
  cryoAsense1stateLast = cryoAsense1state;
  cryoAsense2stateLast = cryoAsense2state;
  cryoAsense1state = digitalRead(cryoAsense1);
  cryoAsense2state = digitalRead(cryoAsense2);
  Serial.print(cryoBsense1state);
  Serial.print(cryoBsense2state);
  cryoBsense1stateLast = cryoBsense1state;
  cryoBsense2stateLast = cryoBsense2state;
  cryoBsense1state = digitalRead(cryoBsense1);
  cryoBsense2state = digitalRead(cryoBsense2);
  Serial.print(cryoBsense1state);
  Serial.print(cryoBsense2state);
  cryoBsense1stateLast = cryoBsense1state;
  cryoBsense2stateLast = cryoBsense2state;
  cryoBsense1state = digitalRead(cryoBsense1);
  cryoBsense2state = digitalRead(cryoBsense2);
}

void reset(){
  delay(5);
  cryoAsense1stateLast = 1;
  cryoAsense2stateLast = 1;
  cryoAsense1state = 1;
  cryoAsense2state = 1;
}

void go(){
    Serial.println("A");
    digitalWrite(cryoAdirection, HIGH);
    digitalWrite(cryoBdirection, HIGH);    
    digitalWrite(cryoAenable, HIGH);
    digitalWrite(cryoBenable, HIGH);
reset();
delay(3000);
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

    goFlag=0;
    goBackFlag=1;
  }
   
void goBack(){
    Serial.println("W");
    digitalWrite(cryoAdirection, HIGH);
    digitalWrite(cryoBdirection, HIGH);    
    digitalWrite(cryoAenable, HIGH);
    digitalWrite(cryoBenable, HIGH);
reset();
delay(3000);
    while ((cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH) &&  (cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    Serial.println("X");
    if (cryoAsense2state == LOW || cryoAsense2stateLast == LOW){
    digitalWrite(cryoAenable, LOW);
reset();
    while ((cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    Serial.println("Y");
    digitalWrite(cryoBenable, LOW);
    }
    else if (cryoBsense2state == LOW || cryoBsense2stateLast == LOW){
    digitalWrite(cryoBenable, LOW);
reset();
    while ((cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH))sense();
    Serial.println("Z");
    digitalWrite(cryoAenable, LOW);
    }
reset();
        goFlag=1;
    goBackFlag=0;
  }

void show1(){
  
}

void show2(){
  
}

void show2(){
  
}
  
void loop() {
  //sense();
  if (goFlag==1)go();
  digitalWrite(cryoAenable, LOW);
  digitalWrite(cryoBenable, LOW);
  delay(5000);
  
  //sense();
  if (goBackFlag==1)goBack();
  digitalWrite(cryoAenable, LOW);
  digitalWrite(cryoBenable, LOW);
  delay(5000);
}
