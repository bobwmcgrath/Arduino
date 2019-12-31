/* roboshelf by Bob*/
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>

//sd file
File myFile;

//motor drive pins
int ENA=5;
int IN1=7;
int IN2=6;

//buttons
int BUTTON_FAN=2;
int BUTTON_LIGHT=9;
int BUTTON_GO=8;
int BUTTON_TEACH=3;
int BUTTON_goUp=A0;
int BUTTON_goDown=4;
int BUTTON_STOP=A1;

//STATE
int  BUTTON_GO_STATE=1;
int  BUTTON_goUp_STATE=1;
int  BUTTON_goDown_STATE=1;
int  BUTTON_TEACH_STATE=1;
int  BUTTON_FAN_STATE=1;
int  BUTTON_LIGHT_STATE=1;
int  BUTTON_STOP_STATE=1;
int  AMPS_STATE;

//SPI for SD card
int CS=10;
//int MOSI=11;
//int MISO=12;
//int CLK=13;

//current sensor
int AMPS=A3;

//relays
int RELAY_FAN=A5;
int RELAY_LIGHT=A2;
int RELAY_BLUE_LIGHT=18;
int RELAY_BREAK=A4;

//variables
int x=10;
bool UP_FLAG;
bool DOWN_FLAG;
int acc=1;
unsigned long setPoint;
int startTime=0;
int currentTime=0;
int RELAY_FAN_FLAG=0;
int RELAY_LIGHT_FLAG=0;
//

void setup()
{
 EEPROM.get(0,setPoint);
 pinMode(ENA,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT); 
  
 pinMode(BUTTON_FAN,INPUT_PULLUP);
 pinMode(BUTTON_LIGHT,INPUT_PULLUP);
 pinMode(BUTTON_GO,INPUT_PULLUP);
 pinMode(BUTTON_TEACH,INPUT_PULLUP);
 pinMode(BUTTON_goUp,INPUT_PULLUP);
 pinMode(BUTTON_goDown,INPUT_PULLUP);
 pinMode(BUTTON_STOP,INPUT_PULLUP);

 pinMode(AMPS,INPUT);

 pinMode (RELAY_FAN,OUTPUT);
 pinMode (RELAY_LIGHT,OUTPUT);
 pinMode (RELAY_BLUE_LIGHT,OUTPUT);
 pinMode (RELAY_BREAK,OUTPUT);

 

 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
}
 sense();
 digitalWrite(RELAY_BLUE_LIGHT,HIGH);
 digitalWrite(RELAY_BREAK,HIGH);
 goHome();
 digitalWrite(RELAY_BREAK,LOW);
Serial.println("setup");
 /* Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
*/}


/*void sdWrite(int setPoint){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("setPoint.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(setPoint);
    setPoint=myFile.read();
    Serial.println(setPoint);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}*/

void goUp(int spd, int x)
{
  
 sense();
 if (BUTTON_STOP_STATE == 0){   
 analogWrite(ENA, spd);// motor speed  
 digitalWrite(IN1,LOW);// rotate forward
 digitalWrite(IN2,HIGH);
 Serial.println("up");
 delay(x);
 analogWrite(ENA, 0);
  
 }
} 

void goDown(int spd, int x)
{ 
  
 analogWrite(ENA, spd);// motor speed    
 digitalWrite(IN1,HIGH);// rotate reverse
 digitalWrite(IN2,LOW);
 delay(x);
 analogWrite(ENA, 0);
 Serial.println("down");
  

} 

void goTo(){
goDown(200,setPoint);
  
}

void goHome(){
  while (BUTTON_STOP_STATE==0){
  sense;
  goUp(200,10);
  Serial.println("home");
  }
}

void teach(){
 
 
 delay(10);
 while (BUTTON_TEACH_STATE==0){
  sense();
  goDown(250,10);
  currentTime=millis();
  Serial.println("teach");
 }  
  setPoint=currentTime-startTime;
  EEPROM.put(0,setPoint);
}

void sense(){
  //delay(100);
  BUTTON_GO_STATE=digitalRead(BUTTON_GO);
  BUTTON_goUp_STATE=digitalRead(BUTTON_goUp);
  BUTTON_goDown_STATE=digitalRead(BUTTON_goDown);
  BUTTON_TEACH_STATE=digitalRead(BUTTON_TEACH);
  BUTTON_LIGHT_STATE=digitalRead(BUTTON_LIGHT);
  BUTTON_FAN_STATE=digitalRead(BUTTON_FAN);
  BUTTON_STOP_STATE=digitalRead(BUTTON_STOP);
  AMPS_STATE=analogRead(AMPS);
  /*Serial.print(BUTTON_GO_STATE);
  Serial.print(BUTTON_goUp_STATE);
  Serial.print(BUTTON_goDown_STATE);
  Serial.print(BUTTON_TEACH_STATE);
  Serial.print(BUTTON_LIGHT_STATE);
  Serial.print(BUTTON_FAN_STATE);
  Serial.print(BUTTON_STOP_STATE);
  Serial.println();*/
  Serial.print(AMPS_STATE);
  Serial.println();
}

void light(){
  delay(10);
  if (RELAY_LIGHT_FLAG==1){digitalWrite(RELAY_LIGHT,LOW);
  RELAY_LIGHT_FLAG=0;}
  else {digitalWrite(RELAY_LIGHT,HIGH);
  RELAY_LIGHT_FLAG=1;}
  while (BUTTON_LIGHT_STATE==0){
    delay(10);
    bloop();
    sense();
  }
  Serial.println("light");
}

void fan(){
  delay(100);
  if (RELAY_FAN_FLAG==1){digitalWrite(RELAY_FAN,LOW);
  RELAY_FAN_FLAG=0;}
  else {digitalWrite(RELAY_FAN,HIGH);
  RELAY_FAN_FLAG=1;}
  while (BUTTON_FAN_STATE==0){
    delay(10);
    bloop();
    sense();
  }
  Serial.println("fan");
}

void bloop(){
  Serial.println("bloop");
}
void loop(){
 delay(100);
 sense();
 if (BUTTON_LIGHT_STATE==0)light();
 if (BUTTON_FAN_STATE==0)fan();
 if (BUTTON_GO_STATE==0 && BUTTON_STOP_STATE==1){
  digitalWrite(RELAY_BREAK,HIGH);
  goTo();
  digitalWrite(RELAY_BREAK,LOW);
 }
 if (BUTTON_goUp_STATE==0){
  digitalWrite(RELAY_BREAK,HIGH);
  while (BUTTON_goUp_STATE==0){goUp(200,10);sense();}
  digitalWrite(RELAY_BREAK,LOW);
 }
 if (BUTTON_GO_STATE==0 && BUTTON_STOP_STATE==0){
  digitalWrite(RELAY_BREAK,HIGH);
  goHome();
  digitalWrite(RELAY_BREAK,LOW);
 }
 if (BUTTON_goDown_STATE==0){
  digitalWrite(RELAY_BREAK,HIGH);
  while (BUTTON_goDown_STATE==0){goDown(200,10);sense();}
  digitalWrite(RELAY_BREAK,LOW);
 }
 if (BUTTON_TEACH_STATE==0 && BUTTON_STOP_STATE==1){
  digitalWrite(RELAY_BREAK,HIGH);
  startTime=millis();
  teach();
  digitalWrite(RELAY_BREAK,LOW);
 }

}
