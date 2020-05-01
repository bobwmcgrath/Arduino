String incomingByte; // for incoming serial data
const int LED=45;
void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);// opens serial port, sets data rate to 9600 bps
}

void loop() {
  digitalWrite(LED,LOW);
  delay(100);
  //Serial.println("butt");
  // send data only when you receive data:
  //if (Serial3.available() > 5) {
    // read the incoming byte:
    incomingByte = Serial3.read();
  digitalWrite(LED,HIGH);
  delay(1100);
    // say what you got:
    Serial.println(incomingByte);
    //Serial.println(incomingByte, DEC);
  //}
}
