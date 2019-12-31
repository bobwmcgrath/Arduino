/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is availabLle using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
*/

// the setup routine runs once when you press reset:
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
    // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  int x = 500;//random(10,100);
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int sensorValue1 = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  // print out the value you read:
  Serial.print(voltage);
  Serial.print(" ");
  Serial.println(voltage1);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(x);    
  sensorValue = analogRead(A0);
  sensorValue1 = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = sensorValue * (5.0 / 1023.0);
  voltage1 = sensorValue1 * (5.0 / 1023.0);
  // print out the value you read:
  Serial.print(voltage);
  Serial.print(" ");
  Serial.println(voltage1);
  // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(x);                       // wait for a second
  
}
