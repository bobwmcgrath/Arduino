

// constants won't change. They're used here to set pin numbers:
const int cryoAsense1 =  A1;    
const int cryoAsense2 =  A0;
const int cryoAdirection =  5;
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
  pinMode(cryoAdirection, OUTPUT);
  pinMode(cryoAenable, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(cryoAsense1, INPUT_PULLUP);
  pinMode(cryoAsense2, INPUT_PULLUP);
  digitalWrite(cryoAenable, LOW);
  digitalWrite(cryoAdirection, HIGH);
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
  
  if (cryoAsense1state == LOW && cryoAsense1stateLast == LOW) {
    digitalWrite(cryoAenable, HIGH);
    digitalWrite(cryoAdirection, LOW);
    delay (4000);
  cryoAsense1stateLast = 1;
  cryoAsense2stateLast = 1;
  cryoAsense1state = 1;
  cryoAsense2state = 1;
    while (cryoAsense2state == HIGH || cryoAsense2stateLast == HIGH)sense();
    goBflag=1;
    goAflag=0;
    digitalWrite(cryoAenable, LOW);
    delay(10000);
    //digitalWrite(cryoAdirection, HIGH);
    //digitalWrite(cryoAenable, HIGH);
  }
} 
void goB(){
    if (cryoAsense2state == LOW && cryoAsense2stateLast == LOW) {
    digitalWrite(cryoAenable, HIGH);
    digitalWrite(cryoAdirection, HIGH);
    delay (4000);
  cryoAsense1stateLast = 1;
  cryoAsense2stateLast = 1;
  cryoAsense1state = 1;
  cryoAsense2state = 1;
    while (cryoAsense1state == HIGH || cryoAsense1stateLast == HIGH)sense();
    goAflag=1;
    goBflag=0;
    digitalWrite(cryoAenable, LOW);
    delay(10000);
    //digitalWrite(cryoAdirection, HIGH);
    //digitalWrite(cryoAenable, HIGH);
    //delay(4000);
    // turn LED on:
    //digitalWrite(ledPin, HIGH);
  }
  
}
  
void loop() {
  sense();
  digitalWrite(cryoAenable, HIGH);
  digitalWrite(cryoAdirection, HIGH);
  goA();
  sense();
  goB();
  //digitalWrite(cryoAenable, HIGH);
  //digitalWrite(cryoAdirection, LOW);
}
