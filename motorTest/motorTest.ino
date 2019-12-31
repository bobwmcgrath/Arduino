//motor drive pins
int ENA=9;
int IN1=7;
int IN2=6;

int now=1;
int before=1;
int amps=1;

void setup() {
 //TCCR1B = TCCR1B & B11111000 | B00000001; // Set PWM frequency for D9 & D10:  pinMode(pwmPin9, OUTPUT); // Sets the pin as output
 
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

void goUp(int spd, int x)
{
    
 
 digitalWrite(IN1,LOW);// rotate forward
 digitalWrite(IN2,HIGH);
 analogWrite(ENA, spd);// motor speed  
 //Serial.println("up");
  before =millis();
 now=millis();
 while (now - before < x){
  now=millis();
  amps=analogRead(A0);
  Serial.println(amps);
 }
 analogWrite(ENA, 0);
  
 }
 
void goDown(int spd, int x)
{
    
  
 digitalWrite(IN1,HIGH);// rotate forward
 digitalWrite(IN2,LOW);
 analogWrite(ENA, spd);// motor speed  
 //Serial.println("down");
 before =millis();
 now=millis();
 while (now - before < x){
  now=millis();
  amps=analogRead(A0);
  Serial.println(amps);
 }
 analogWrite(ENA, 0);
  
 }


void loop() {
  goUp(200,5000);
  delay(1000);
  goDown(200,5000);
  delay(1000);

}
