// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define LED_COUNT  3
#define BRIGHTNESS 255

int zero[3]  = {1,0,0};
int one[3]   = {0,1,0};
int two[3]   = {0,1,1};
int three[3] = {0,0,0};

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  // Fill along the length of the strip in various colors...
  colorWipe(three); // True white (not RGB white)
  delay(1000);                           //  Pause for a moment
  colorWipe(one); // True white (not RGB white)
  delay(1000);                           //  Pause for a moment
}

void colorWipe(int num[3]) {
  uint32_t color;
  for(int i=0; i<sizeof(num); i++) { // For each pixel in strip...
    if (num[i]==0)color=strip.Color(  0,   0,   0, 0);
    if (num[i]==1)color=strip.Color(  220,   5,   0, 255);
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }


  //Serial.println(sum(pix[num]));
}
