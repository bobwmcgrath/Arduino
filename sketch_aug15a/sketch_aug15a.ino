/*
   Kilburn NERF Experience Basketball Master
   Contributors: Joe Mertz and Bob McGrath @ L3DFX in Bolingbrook, Illinois
        Joe Mertz www.github.com/jcmertz
        Bob McGrath www.github.com/bobwmcgrath
        L3DFX www.l3dfx.com
   Version 1.0
   Date: 7.30.19
   Version Notes:
   This code is designed to be run on the ESP32 chipset, using the L3DFX proprietary
   breakout board for IO as well as the Wiznet 5500 chipset for ethernet communication.
*/

#define DEBUG

#define SENSORPIN 14
#define THRESHOLD 5


//Librarys to be included
#include <SPI.h> // Required for Ethernet
#include <Ethernet.h>
#include <PubSubClient.h> //This is the MQTT Library

// Debugging Code. Enables debug printer if DEBUG flag is enabled
#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif


//<-------------------------------------------------------------------------------------------------------------------------------------------
// mac and MQTT_ID must be unique for each device in an activation

byte mac[]    = {  0xDD, 0xED, 0xBA, 0xAB, 0xFE, 0x06 };
char clientName[] = "basketball4";

//<-------------------------------------------------------------------------------------------------------------------------------------------
const char* MQTT_ID = (char*)mac;


//MQTT Broker/Server IP address
IPAddress server(10, 100, 100, 10); // This should be the same within each activation

int targetMode, playerNumber, pointValue;


//Set Up MQTT Client
EthernetClient ethClient;
PubSubClient client(ethClient);


int sensorState = 0, lastState = 0;      // variable for reading the breakbeam status
int brokenCount; // variable used for debouncing. Stores how many loop cycles the break beam has been broken for

bool gameOn = false;


void callback(char* topic, byte* payload, unsigned int length) {
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  for (int i = 0; i < length; i++) {
    DEBUG_PRINTLN((char)payload[i]);
  }
  if ((String)topic == "activation/reset")
    gameOn = false;
  else if ((String)topic == (String)clientName + "/getParams") {
    String base = "activation/targets/";
    publishToTopic(base + clientName, "PARAMS: " + (String)targetMode + (String)playerNumber + (String)pointValue + (String)clientName);
  }
  else if ((String)topic == "timer/start") {
    gameOn = true;
  }
  else if ((String)topic == "timer/end") {
    gameOn = false;
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
      client.subscribe("timer/start");
      client.subscribe("timer/end");
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

void setup() {

  // initialize the sensor pin as an input:
  pinMode(SENSORPIN, INPUT_PULLUP);
  digitalWrite(SENSORPIN, HIGH); // turn on the pullup

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


  // Load config from IP ADDRESS
  DEBUG_PRINTLN(Ethernet.localIP());
  DEBUG_PRINTLN(Ethernet.gatewayIP());
  int index = Ethernet.localIP().toString().indexOf('.') + 1;
  int endex = Ethernet.localIP().toString().indexOf('.', index);
  String octet = Ethernet.localIP().toString().substring(index, endex);
  DEBUG_PRINTLN(octet);
  targetMode = octet.toInt() / 10;
  playerNumber = octet.toInt() % 10;
  DEBUG_PRINTLN(playerNumber);
  index = Ethernet.localIP().toString().indexOf('.', endex) + 1;
  endex = Ethernet.localIP().toString().indexOf('.', index);
  octet = Ethernet.localIP().toString().substring(index, endex);
  DEBUG_PRINTLN(octet);
  pointValue = octet.toInt();



  // Establish MQTT Broker connection

  if (!client.connected()) {
    reconnect();
  }
}

void loop() {
  // read the state of the pushbutton value:
  sensorState = digitalRead(SENSORPIN);
  Serial.println(sensorState);
  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:


  if (sensorState && !lastState && brokenCount > THRESHOLD) {
    DEBUG_PRINTLN("Unbroken");
    if (gameOn) {
      String base = "player";
      publishToTopic(base + playerNumber + "/score", (String)pointValue);
      DEBUG_PRINTLN("playerNumber");
      delay(500);
    }
  }
  if (sensorState)
    brokenCount = 0;
  if (!sensorState) {
    brokenCount++;
    DEBUG_PRINT("Count: ");DEBUG_PRINTLN(brokenCount);
  }

  lastState = sensorState;

  // check MQTT messages
  client.loop();
  // if reset: reset to top of game
  // if MQTT_ID: load new params
}
