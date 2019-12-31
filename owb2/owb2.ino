#include <OneWire.h>

#define DS2413_ONEWIRE_PIN  (8)

#define DS2413_FAMILY_ID    0x29
#define DS2413_ACCESS_READ  0xF5
#define DS2413_ACCESS_WRITE 0x5A
#define DS2413_ACK_SUCCESS  0xAA
#define DS2413_ACK_ERROR    0xFF

OneWire oneWire(DS2413_ONEWIRE_PIN);
uint8_t address[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };



byte read(void)
{    
  bool ok = false;
  uint8_t results;

  oneWire.reset();
  oneWire.select(address);
  oneWire.write(DS2413_ACCESS_READ);

  results = oneWire.read();                 /* Get the register results   */
  ok = (!results & 0x0F) == (results >> 8); /* Compare nibbles            */
  results &= 0x0F;                          /* Clear inverted values      */

  oneWire.reset();
  
  // return ok ? results : -1;
  return results;
}



void setup(void) 
{
  Serial.begin(9600);  
  
  Serial.println(F("Looking for a DS2413 on the bus"));
  
  /* Try to find a device on the bus */
  oneWire.reset_search();
  delay(250);

  
  /* Check the CRC in the device address */
  if (OneWire::crc8(address, 7) != address[7]) 
  {
    Serial.println(F("Invalid CRC!"));
    while(1);
  }
  
}
  
  


void loop(void) 
{
  /* Read */
  
  uint8_t state = read();
  if (state == -1)
    Serial.println(F("Failed reading the DS2413"));
  else
    Serial.println(state, DEC);
  delay(500);
  /* Write */
  //bool ok = false;
  //ok = write(0x3);
  //if (!ok) Serial.println(F("Wire failed"));
  //delay(500);
  //ok = write(0x0);
  //if (!ok) Serial.println(F("Wire failed"));
  //delay(500);
}
