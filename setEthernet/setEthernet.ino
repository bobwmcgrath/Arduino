#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 202);

void setup() {
  
  Ethernet.begin(mac, ip);
  //IPAddress newIp(192, 168, 1, 82);
  //3Ethernet.setLocalIP(newIp); 
  Serial.begin(115200);
  // change the IP address
}

void loop () {
    auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }
  delay(1000);
  Serial.println(Ethernet.localIP());
  }
