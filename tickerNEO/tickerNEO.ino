// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define LED_COUNT  3
#define BRIGHTNESS 255
              //1 2 3 4 5 6 7

int digit(int num){
  int output[7] = {1,1,1,1,1,1,1};
  if (num==0)output[7] = {0,1,1,1,1,1,1};
  if (num==1)output[7]   = {0,0,0,0,1,1,0};
  if (num==2)output[7]   = {1,1,1,0,1,1,0};
  if (num==3)output[7] = {1,0,1,1,1,1,1};
  if (num==4)output[7]  = {1,0,0,1,1,0,1};
  if (num==5)output[7]  = {1,0,1,1,0,1,1};
  if (num==6)output[7]   = {0,0,0,1,1,1,0};
  if (num==7)output[7] = {1,1,1,1,1,1,1};
  if (num==8)output[7] = {1,1,1,1,1,1,1};
  if (num==9)output[7]  = {1,0,0,1,1,1,1};
  return output[]
}


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
  colorWipe(digit(8)); // True white (not RGB white)
  delay(1000);                           //  Pause for a moment
  colorWipe(digit(7)); // True white (not RGB white)
  delay(1000);                           //  Pause for a moment
}

void colorWipe(int num[7]) {
  uint32_t color;
  for(int i=0; i<7; i+=2) { // For each pixel in strip...
    if (num[i]==0)color=strip.Color(  0,   0,   0, 0);
    if (num[i]==1)color=strip.Color(  220,   5,   0, 255);
    strip.setPixelColor(i, color);
    strip.show();
    strip.setPixelColor(i+1, color);
    strip.show();
  }


  //Serial.println(sum(pix[num]));
}
