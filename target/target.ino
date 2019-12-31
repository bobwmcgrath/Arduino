int echoRead=13
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(12, OUTPUT);
  pinMode(echoRead, INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  trig();
  while echoRead=0 echo();   
}

void trig() {
  digitalWrite(12, HIGH);   
  delayMicroseconds(11);                      
  digitalWrite(12, LOW);   
}

void echo(){

  digitalRead(echoRead)
  
}
