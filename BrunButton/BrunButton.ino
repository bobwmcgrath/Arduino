//Which input pin is the start/stop button connected to?
const int bigButton = 34;
//Which input pin is the start/stop button LED connected to?
const int bigButtonLED = 32;
//Which input pin is the reset button connected to?
const int resetButton = 26;
//game on or off
int gameOn = 0;

void setup() {
  
  pinMode(bigButtonLED, OUTPUT);
  pinMode(bigButton, INPUT);
  pinMode(resetButton, INPUT);
  digitalWrite (bigButtonLED, HIGH); 
}

void loop() {
  if (gameOn == 0) 
  {digitalWrite(bigButtonLED, HIGH);
  senseBigButton();}
  if (gameOn == 1)
  {digitalWrite(bigButtonLED, LOW);
  senseResetButton();}
  if (GAME END/RESET MESSAGE RECIEVED)gameOn = 0;
}

void senseBigButton() {
  bigButtonState = digitalRead(bigButton);
  if (bigButtonState == HIGH) 
  {SEND START;
  gameOn=1;}
  
}
void senseResetButton() {
  resetButtonState = digitalRead(bigButton);
  if (resetButtonState == HIGH) 
  {SEND RESET;
  gameOn=0;}
}
}
