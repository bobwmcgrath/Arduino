/*
   Kilburn NERF Experience Target Master
   Contributors: Joe Mertz and Bob McGrath @ L3DFX
        Joe Mertz www.github.com/jcmertz
        Bob McGrath www.github.com/bobwmcgrath
        L3DFX www.l3dfx.com
   Version 1.2
   Date: 7.29.19
   Version Notes: This version is being pushed to the first dozen ESPs for LA MArketing.
   This code is being developed for L3DFX and the Kilburn Media Nerf Experience.
   Specifically this code is to be used for the primary interchangeable target
   boards in the following activations:
   - Blaster Run
   - Dodge Blast
   - Try-Athalon
   - Vortex Throw
   - Bash Ball Wall
   This code is designed to be run on the ESP32 chipset, using the L3DFX proprietary
   breakout board for IO as well as the Wiznet 5500 chipset for ethernet communication.
*/
#define debug

//Librarys to be included
#include <SPI.h> // Required for Ethernet
#include <Ethernet.h>
#include <PubSubClient.h> //This is the MQTT Library
#include <Adafruit_NeoPixel.h> //For driving the LED ring or strip

//Define some constant variables

#define THRESHOLD 180  //180 was original. Threshold of what counts as a hit for the target
#define DEBOUNCE 100  //100 was original. How long to wait to detect double hits

#define LED_PIN     16  // Which pin on the Arduino is connected to the NeoPixels?
#define LED_COUNT  60  // How many NeoPixels are attached to the Arduino?


//<-------------------------------------------------------------------------------------------------------------------------------------------
// mac and MQTT_ID must be unique for each device in an activation

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x09` };
char clientName[] = "blastoise";

//<-------------------------------------------------------------------------------------------------------------------------------------------
const char* MQTT_ID = (char*)mac;


//MQTT Broker/Server IP address
IPAddress server(10, 100, 100, 10); // This should be the same within each activation

//Which input pin is the target piezo connected to?
const int sensorPin = 36;

//Values used to store data received via encoding in the IP Address
int targetMode, playerNumber, pointValue;


//Set Up MQTT Client
EthernetClient ethClient;
PubSubClient client(ethClient);

//Debounce code variables
long lastStrike;
int lastTime = 0;

// Debugging Code. Enables debug printer if DEBUG flag is enabled
#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.print (x)
  #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif


//This function needs to change depending on the type of sensor used (piezo, break beam, etc.)
bool sensorTripped() {
  // read the input on the sensor pin:
  int sensorValue = analogRead(sensorPin);
  DEBUG_PRINT("Sensor Value: "); DEBUG_PRINTLN(sensorValue);
  if (sensorValue > THRESHOLD) {
    return true;
  }
  return false;
}


// --- FUNCTIONS AND VARIABLES BELOW THIS LINE SHOULD REMAIN UNCHANGED FOR ALL TARGETS ------ //


// Declare our NeoPixel ring object:
Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel ring
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)



void setup() {
  Serial.begin(57600);

  //Init MQTT
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
      delay(5000);
      ESP.restart();
  }
  // Allow the hardware to sort itself out
  delay(1500);

  //  //Init EEProm
  //  EEPROM.begin(EEPROM_SIZE);

  // Init Sensor

  // Init Indicators

  ring.begin();           // INITIALIZE NeoPixel ring object (REQUIRED)
  ring.clear();
  ring.show();            // Turn OFF all pixels ASAP
  ring.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Load config from IP ADDRESS
  DEBUG_PRINTLN(Ethernet.localIP());
  DEBUG_PRINTLN(Ethernet.gatewayIP());
  int index = Ethernet.localIP().toString().indexOf('.') + 1;
  int endex = Ethernet.localIP().toString().indexOf('.', index);
  String octet = Ethernet.localIP().toString().substring(index, endex);
  DEBUG_PRINTLN(octet);
  targetMode = octet.toInt() / 100;
  playerNumber = octet.toInt() % 100;
  index = Ethernet.localIP().toString().indexOf('.', endex) + 1;
  endex = Ethernet.localIP().toString().indexOf('.', index);
  octet = Ethernet.localIP().toString().substring(index, endex);
  DEBUG_PRINTLN(octet);
  pointValue = octet.toInt();

  DEBUG_PRINTLN(playerNumber);

  // Establish MQTT Broker connection and subscribe to reset topic

  if (!client.connected()) {
    reconnect();
  }
  resetTarget();
}

void loop() {
  // Ensure MQTT Still Connected
  if (!client.connected()) {
    reconnect();
    Ethernet.maintain();
    DEBUG_PRINTLN(Ethernet.localIP());
    DEBUG_PRINTLN(Ethernet.gatewayIP());
  }

  // Check sensor
  // if tripped:
  if (debounce(lastStrike) && sensorTripped()) {
    DEBUG_PRINTLN("HIT");
    lastStrike = millis();
    indicateHit();
  }

  // check MQTT messages
  client.loop();
  // if reset: reset to top of game
  // if MQTT_ID: load new params
  //
}

bool debounce(long lastStrike) {
  long strikeTime = millis();
  if (strikeTime - lastStrike > DEBOUNCE) {
    return true;
  }
  return false;
}

void indicateHit() {
  String base = "player";
  publishToTopic(base + playerNumber + "/score", (String)pointValue);
  DEBUG_PRINTLN(playerNumber);
  explode(2);
}

void resetTarget() {
  colorWipe(ring.Color(250,   107,   02)     , 2); // Orange
  colorWipe(ring.Color(  5, 52,   184)     , 2); // Blue
}
void callback(char* topic, byte* payload, unsigned int length) {
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  for (int i = 0; i < length; i++) {
    DEBUG_PRINTLN((char)payload[i]);
  }
  if ((String)topic == "activation/reset")
    resetTarget();
  if ((String)topic == (String)clientName + "/getParams") {
    String base = "activation/targets/";
    publishToTopic(base + clientName, "PARAMS: " + (String)targetMode + (String)playerNumber + (String)pointValue + (String)clientName);

  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_ID)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("activation/reset");
      char temp[50];
      DEBUG_PRINTLN(clientName);
      strcat(temp, clientName);
      DEBUG_PRINTLN(temp);
      strcat(temp, "/+");
      DEBUG_PRINTLN(temp);
      client.subscribe(temp);
      String base = "activation/targets/";
      publishToTopic(base + clientName, (String) clientName + " is here!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      Ethernet.maintain();
      DEBUG_PRINTLN(Ethernet.localIP());
      DEBUG_PRINTLN(Ethernet.gatewayIP());
    }
  }
}
void publishToTopic(String subTopic, String message) {
  char subTopicOut[subTopic.length() + 1];
  subTopicOut[subTopic.length()] = '\0';
  subTopic.toCharArray(subTopicOut, subTopic.length() + 1);

  char messageOut[message.length() + 1];
  messageOut[message.length()] = '\0';
  message.toCharArray(messageOut, message.length() + 1);

  client.publish(subTopicOut, messageOut);
}

void colorWipe( uint32_t color, int wait) {
  for (int i = 0; i < ring.numPixels(); i++) { // For each pixel in ring...
    ring.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    ring.show();
    delay(wait);
  }

}

void explode(int fadeSpeed) {

  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;
  int brightness = 255;
  for (int i = 0; i < 3; i++) {
    ring.fill(ring.Color(0, 0, 0, 255), 0, ring.numPixels());
    ring.show();
    delay(50);
    ring.clear();
    ring.show();
    delay(50);
  }
  for (;;) { // Repeat forever (or until a 'break' or 'return')
    for (int i = 0; i < ring.numPixels(); i++) { // For each pixel in ring...
      int pixelHue = firstPixelHue + (i * 65536L / ring.numPixels());
      ring.setPixelColor(i, ring.gamma32(ring.ColorHSV(pixelHue, 255, brightness)));
    }

    ring.show(); // Update ring with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 400; // Advance just a little along the color wheel

    if (brightness < 10) {
      ring.clear();
      ring.show();
      ring.clear();
      ring.show();
      return;
    }
    if ((millis() - lastTime) > fadeSpeed) { // Time to update head/tail?
      brightness = brightness - 2;
    }
  }
}
