void setup()
{
  //Arduino Nano also has a builtin LED and a Macro to use it.
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  for(int i = 0; i < 255; i++)
  {int   x=(i);
    //Set the LED pin to HIGH. This gives power to the LED and turns it on
    analogWrite(45, x);
    delay(10);
  }
    for(int i = 0; i < 255; i++)
  {int   x=(255-i);
    //Set the LED pin to HIGH. This gives power to the LED and turns it on
    analogWrite(45, x);
    delay(10);
  }
}
