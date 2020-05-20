#ifndef OWB_h
#define OWB_h

#include <OneWire.h>
#include "CONST.h"

//OWB stuff
#define DS2408_ONEWIRE_PIN  (8)
#define DS2408_FAMILY_ID    0x29
#define DS2408_ACCESS_READ  0xF5
#define DS2408_ACCESS_WRITE 0x5A
#define DS2408_ACK_SUCCESS  0xAA
#define DS2408_ACK_ERROR    0xFF

namespace owb{
  

void printBytes(uint8_t* addr, uint8_t count, bool newline=0);
byte read(void);
bool write(uint8_t state);
void OWB_setup();

}
#endif
