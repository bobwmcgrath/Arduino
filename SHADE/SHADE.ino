

// constants won't change. They're used here to set pin numbers:
const int cryoAsense1 =  11;    
const int cryoAsense2 =  10;
const int cryoAdirection =  5;
const int cryoAenable =  4;
const int cryoBsense1 =  13;    
const int cryoBsense2 =  12;
const int cryoBdirection =  9;
const int cryoBenable =  8;

// variables will change:
int cryoAsense1state = 1;
int cryoAsense2state = 1;// variable for reading the sensor status
int cryoAsense1stateLast = 1;//variables for debouncing
int cryoAsense2stateLast = 1;
int cryoBsense1state = 1;
int cryoBsense2state = 1;
int cryoBsense1stateLast = 1;
int cryoBsense2stateLast = 1;
int goFlag = 1;
int goBackFlag = 1;

void setup() {
  Serial.begin(9600);
  pinMode(cryoAdirection, OUTPUT);
  pinMode(cryoAenable, OUTPUT);
  pinMode(cryoBdirection, OUTPUT);
  pinMode(cryoBenable, OUTPUT);
  pinMode(cryoAsense1, INPUT_PULLUP);
  pinMode(cryoAsense2, INPUT_PULLUP);
  pinMode(cryoBsense1, INPUT_PULLUP);
  pinMode(cryoBsense2, INPUT_PULLUP);
  digitalWrite(cryoAenable, LOW);
  digitalWrite(cryoAdirection, HIGH);
  digitalWrite(cryoBenable, LOW);
  digitalWrite(cryoBdirection, HIGH);
}


void sense() {
  //Serial.println(cryoAsense1state);
  //Serial.print(cryoAsense2state);
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
}

void reset(){
  delay(10);
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
    while ((cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH) &&  (cryoBsense1state == HIGH || cryoBsense1stateLast == HIGH))sense();
    Serial.println("B");
    if (cryoAsense1state == LOW || cryoAsense1stateLast == LOW){
    digitalWrite(cryoAenable, LOW);
    digitalWrite(cryoAdirection, LOW);
//reset();
    while ((cryoBsense1state == HIGH || cryoBsense1stateLast == HIGH))sense();
    Serial.println("C");
    digitalWrite(cryoBdirection, LOW);
    digitalWrite(cryoAenable, HIGH);
    }
    if (cryoBsense1state == LOW && cryoBsense1stateLast == LOW){
    digitalWrite(cryoBenable, LOW);
//reset();
    while ((cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH))sense();
    Serial.println("D");
    digitalWrite(cryoAdirection, LOW);
    digitalWrite(cryoBenable, HIGH);
    }
//reset();
    while ((cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH) ||  (cryoBsense1state == HIGH || cryoBsense1stateLast == HIGH))sense();
    Serial.println("E");
    if (cryoAsense1state == LOW && cryoAsense1stateLast == LOW){
    digitalWrite(cryoAenable, LOW);
//reset();
    while ((cryoBsense1state == HIGH || cryoBsense1stateLast == HIGH))sense();
    Serial.println("F");
    digitalWrite(cryoBenable, LOW);
    }
    if (cryoBsense1state == LOW && cryoBsense1stateLast == LOW){
    digitalWrite(cryoBenable, LOW);
//reset();
    while ((cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH))sense();
    Serial.println("G");
    digitalWrite(cryoBenable, LOW);
    }
    goFlag=0;
    goBackFlag=1;
  }
   
void goBack(){
    digitalWrite(cryoAdirection, HIGH);
    digitalWrite(cryoBdirection, HIGH);    
    digitalWrite(cryoAenable, HIGH);
    digitalWrite(cryoBenable, HIGH);
reset();
    while ((cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH) &&  (cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    if (cryoAsense2state == LOW || cryoAsense2stateLast == LOW){
    digitalWrite(cryoAenable, LOW);
    digitalWrite(cryoAdirection, LOW);
//reset();
    while ((cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    digitalWrite(cryoBdirection, LOW);
    digitalWrite(cryoAenable, HIGH);
    }
    if (cryoBsense2state == LOW || cryoBsense2stateLast == LOW){
    digitalWrite(cryoBenable, LOW);
//reset();
    while ((cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH))sense();
    digitalWrite(cryoAdirection, LOW);
    digitalWrite(cryoBenable, HIGH);
    }
//reset();
    while ((cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH) ||  (cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    if (cryoAsense2state == LOW || cryoAsense2stateLast == LOW){
    digitalWrite(cryoAenable, LOW);
//reset();
    while ((cryoBsense2state == HIGH || cryoBsense2stateLast == HIGH))sense();
    digitalWrite(cryoBenable, LOW);
    }
    if (cryoBsense2state == LOW || cryoBsense2stateLast == LOW){
    digitalWrite(cryoBenable, LOW);
//reset();
    while ((cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH))sense();
    digitalWrite(cryoBenable, LOW);
    }
        goFlag=1;
    goBackFlag=0;
  }

  
  
void loop() {
  //sense();
  if (goFlag==1)go();
  delay(5000);
  //sense();
  if (goBackFlag==1)goBack();
  delay(5000);
}
