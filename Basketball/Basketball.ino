 #include "DualG2HighPowerMotorShield.h"

// Uncomment the version corresponding with the version of your shield.
DualG2HighPowerMotorShield24v14 md;
// DualG2HighPowerMotorShield18v18 md;
// DualG2HighPowerMotorShield24v18 md;
// DualG2HighPowerMotorShield18v22 md;

const int hall0 = 19;
const int hall1 = 20;

int x = 100;
int y = x;
int randNumber = 1;

int hallstate0 = 1;
int hallstate1 = 1;
void stopIfFault()
{
  if (md.getM1Fault())
  {
    md.disableDrivers();
	delay(1);
    Serial.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault())
  {
    md.disableDrivers();
	delay(1);
    Serial.println("M2 fault");
    while (1);
  }
}

void setup()
{
  pinMode(hall0, INPUT);
  pinMode(hall1, INPUT);
  Serial.begin(115200);
  Serial.println("Dual G2 High Power Motor Shield");
  md.init();
  md.calibrateCurrentOffsets();

  delay(10);
    md.enableDrivers();
  delay(1);  // The drivers require a maximum of 1ms to elapse when brought out of sleep mode.
  md.setM1Speed(x);
  //Serial.println("delay");
  //delay (3000);
  //Serial.println("delay end");

  // Uncomment to flip a motor's direction:
  //md.flipM1(true);
  //md.flipM2(true);
}

void loop()
{
  hallstate0 = digitalRead(hall0);
  Serial.println(hallstate0);
  hallstate1 = digitalRead(hall1);
  Serial.print(hallstate1);
  if (hallstate1==1 && hallstate0==0)
  {md.setM1Speed(-x);
  y=-x;
  }
  if (hallstate1==0 && hallstate0==1)
  {md.setM1Speed(x);
  y=x;
  }
  if (hallstate1==1 && hallstate0==1)
  {delay(1);
  randNumber = random(20000);
  if (randNumber==0)
  {y *= -1;
  md.setM1Speed(y);
  }
  }}
