//motor drive pins
int ENA=9;
int IN1=7;
int IN2=6;

int encoder0PinA = 2;
int encoder0PinB = 3;

int now=1;
int before=1;
int amps=1;
bool GO_UP_FLAG=0;
bool GO_DOWN_FLAG=0;

volatile unsigned int encoder0Pos = 0;

void setup() {
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

 pinMode(encoder0PinA, INPUT_PULLUP);
 //digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
 pinMode(encoder0PinB, INPUT_PULLUP);
 //digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor
 attachInterrupt(digitalPinToInterrupt(3), doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

 
 pinMode(ENA,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT);
 pinMode(A0,INPUT);
 
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
}
}

// PWM output @ 25 kHz, only on pins 9
// Output value should be between 0 and 320, inclusive.
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
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.

     For more information on speeding up this process, see
     [Reference/PortManipulation], specifically the PIND register.
  */
  if (GO_DOWN_FLAG==1) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }

  //Serial.println (encoder0Pos, DEC);
}



void goUp(int spd, int x, int start)
{
    GO_UP_FLAG=1;
 
 digitalWrite(IN1,LOW);// rotate forward
 digitalWrite(IN2,HIGH);
 delay(10);
 analogWrite( ENA, 0);// motor speed  
 //Serial.println("up");
  before =millis();
 now=millis();
 while (now - before < x){
  delay(10);
  if (spd>start)start++;
  analogWrite( ENA, start);
  now=millis();
  amps=analogRead(A0);
  //Serial.println(amps);
  int y=digitalRead(2);
  Serial.println (y);
 }
 analogWrite(ENA, 0);
  GO_UP_FLAG=0;
 }
 
void goDown(int spd, int x, int start)
{
 GO_DOWN_FLAG=1; 
 digitalWrite(IN1,HIGH);// rotate forward
 digitalWrite(IN2,LOW);
 delay(10);
 analogWrite( ENA, start);// motor speed  
 //Serial.println("down");
 before =millis();
 now=millis();
 while (now - before < x){
  delay(10);
  if (spd>start)start++;
  analogWrite( ENA, start);
  now=millis();
  amps=analogRead(A0);
  int y=digitalRead(2);
  Serial.println(y);
  //Serial.println (encoder0Pos, DEC);
 }
 analogWrite(ENA, 0);
  GO_DOWN_FLAG=0;
 }


void loop() {
  encoder0Pos=0;
  goDown(100,3000,0);
  //delay(1000);
  goUp(100,3000,0);
  //delay(1000);

}
