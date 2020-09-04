#include "Buttons.h"

const int but1pin = A1;
const int but2pin = A2;
const int but3pin = A3;

volatile bool but1pin_state = 1;
volatile bool but2pin_state = 1;
volatile bool but3pin_state = 1;

const int led1pin = 11;
const int led2pin = 12;
const int led3pin = 13;

// Button objects
buttons::PressHoldButton but1(but1pin_state, 2000);
buttons::DebouncedButton but2(but2pin_state);
buttons::DebouncedButton but3(but3pin_state);



void setup() {
  Serial.begin(9600);
  pinMode(but1pin, INPUT_PULLUP);
  pinMode(but2pin, INPUT_PULLUP);
  pinMode(but3pin, INPUT_PULLUP);
  pinMode(led1pin, OUTPUT);
  pinMode(led2pin, OUTPUT);
  pinMode(led3pin, OUTPUT);
  digitalWrite(led1pin, HIGH);
  digitalWrite(led2pin, HIGH);
  digitalWrite(led3pin, HIGH);

}

void loop() {
  but1.update();
  but1pin_state = !digitalRead(but1pin);
  but2pin_state = !digitalRead(but2pin);
  but3pin_state = !digitalRead(but3pin);
  //Serial.println(digitalRead(led1pin));
  if (but1.isHeld()) {digitalWrite(led1pin, HIGH);}
  else{digitalWrite(led1pin, LOW);}
  if (but2.isPressed()) {digitalWrite(led2pin, !digitalRead(led2pin));}
  if (but3.isPressed()) {digitalWrite(led3pin, !digitalRead(led3pin));}
}
