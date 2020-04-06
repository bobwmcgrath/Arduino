void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
    Serial.print(100); //speed
    Serial.print(",");
    Serial.println(100); //position
    delay(3000);
    Serial.print(100);
    Serial.print(",");
    Serial.println(0);
    delay(3000);
    
  // put your main code here, to run repeatedly:

}
