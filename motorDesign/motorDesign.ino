#include "CONST.h"
#include "INI.h"
#include "PINS.h"
#include "motor.h"

int setVel=100;

void doEncoder() //interupt based encoder ticks
{
  if (pins::GDF==0) {
    ticks++;
  } else {
    ticks--;
  }
}

void setup() {
 Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
// } 
 pins::PWMconfig();
 pins::pinINI();
 attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
 Serial.println("SETUP");
}

void loop() {
  //if ((millis()%100000)>5) setVel *= -1;
  delay(10);
  //motor::getVelocity();
  pins::analogWrite25k(ENA, 20);
  Serial.println((millis()%10));
}
