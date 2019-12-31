

// constants won't change. They're used here to set pin numbers:
const int cryoAsense1 =  A5;    
const int cryoAsense2 =  A4;
//const int cryoAdirection =  5;
const int cryoAenable =  4;
;


// variables will change:
int cryoAsense1state = 1;
int cryoAsense2state = 1;// variable for reading the sensor status
int cryoAsense1stateLast = 1;//variables for debouncing
int cryoAsense2stateLast = 1; 
int goAflag = 1;
int goBflag = 1;

void setup() {
  Serial.begin(9600 );
  // initialize the LED pin as an output:
 // pinMode(cryoAdirection, OUTPUT);
  pinMode(cryoAenable, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(cryoAsense1, INPUT_PULLUP);
  pinMode(cryoAsense2, INPUT_PULLUP);
  digitalWrite(cryoAenable, LOW);
  //digitalWrite(cryoAdirection, HIGH);
}


void sense() {
  Serial.println(cryoAsense1state);
  Serial.print(cryoAsense2state);
  cryoAsense1stateLast = cryoAsense1state;
  cryoAsense2stateLast = cryoAsense2state;
  cryoAsense1state = digitalRead(cryoAsense1);
  cryoAsense2state = digitalRead(cryoAsense2);
  //digitalWrite(cryoAenable, HIGH);
}

void goA(){
  digitalWrite(cryoAenable, HIGH);
  delay(1000);
  cryoAsense1stateLast = 1;
  cryoAsense2stateLast = 1;
  cryoAsense1state = 1;
  cryoAsense2state = 1;
  while (cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH) sense();
    digitalWrite(cryoAenable, LOW);
    //digitalWrite(cryoAdirection, LOW);
    delay (7000);
    goBflag=1;
    goAflag=0;
    //digitalWrite(cryoAdirection, HIGH);
    //digitalWrite(cryoAenable, HIGH);
} 
void goB(){  
  digitalWrite(cryoAenable, HIGH);
  delay(1000);
  cryoAsense1stateLast = 1;
  cryoAsense2stateLast = 1;
  cryoAsense1state = 1;
  cryoAsense2state = 1;
  while (cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH) sense();
    digitalWrite(cryoAenable, LOW);
    //digitalWrite(cryoAdirection, LOW);
    delay (7000);
    goBflag=0;
    goAflag=1;
  }
  

  
void loop() {
  sense();
  //digitalWrite(cryoAenable, HIGH);
  //digitalWrite(cryoAdirection, LOW);
  if (goAflag==1)goA();
  sense();
  if (goBflag==1)goB();
  //digitalWrite(cryoAenable, HIGH);
  //digitalWrite(cryoAdirection, LOW);
}
