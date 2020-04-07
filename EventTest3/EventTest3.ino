int pos = 0;
bool flag=0;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
    Serial.print(100); //speed
    Serial.print(",");
    Serial.println(pos); //position
    delay(10);
    if (pos==0)flag=1;
    if (flag==1) pos++;
    if (pos==100)flag=0;
    if (flag==0) pos--;

  // put your main code here, to run repeatedly:

}
