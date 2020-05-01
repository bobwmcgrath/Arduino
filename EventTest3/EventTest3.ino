int pos = 60;
bool flag=0;
int dly=50;
int vel=0;

const int buttonPin =  12;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status


void setup() {
  //pinMode(buttonPin, INPUT_PULLUP);
  
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
    dly=50;
    //buttonState = digitalRead(buttonPin);
    if (buttonState==0)dly=12;
    Serial.print(vel); //speed
    Serial.print(",");
    Serial.println(pos); //position
    delay(dly);
    if (pos==30)flag=1;
    if (flag==1) pos+=1;
    if (pos==100)flag=0;
    if (flag==0) pos-=1;
    vel=1000/dly;
    if (flag==0) vel*=-1;
    

  // put your main code here, to run repeatedly:

}
