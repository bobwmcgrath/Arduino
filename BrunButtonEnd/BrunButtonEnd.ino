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
  digitalWrite (bigButtonLED, LOW); 
}

void loop() {
  if (gameOn == 0) 
  {digitalWrite(bigButtonLED, LOW);
  }
  if (gameOn == 1)
  {digitalWrite(bigButtonLED, HIGH);
  senseBigButton();
  senseResetButton();}
  if (GAME START MESSAGE RECIEVED)gameOn = 1;
}

void senseBigButton() {
  bigButtonState = digitalRead(bigButton);
  if (bigButtonState == HIGH) 
  {SEND END;
  gameOn=0;}
  
}
void senseResetButton() {
  resetButtonState = digitalRead(bigButton);
  if (resetButtonState == HIGH) 
  {SEND RESET;
  gameOn=0;}
}
}
