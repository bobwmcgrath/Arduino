#include <Adafruit_NeoPixel.h>
//#include "Particle.h"
//#include "neopixel.h"

////////////////////////////////////////////////////////////////////////////////
#define NUMPIXELS 14 
#define DATAPIN0 6 


Adafruit_NeoPixel strip=(NUMPIXELS, DATAPIN0, NEO_GRBW + NEO_KHZ800);
const int bright = 100; //adjust brightness for all pixels 0-255 range,
// 32 being pretty dim, 255 being full brightness
////////////////////////////////////////////////////////////////////////////////

void setup() {
////////////////////////////////////////////////////////////////////////////////
  delay(500); //pause a moment to let capacitors on board settle
  //NeoPixel array setup
   strip.begin(); // Initialize pins for output
   strip.show();
   strip.setBrightness(bright);
   //
   delay(200);

  //flash an eight

  delay(1500);
//END void setup()
////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
void loop() {
    colorWipe(strip.Color(  0,   0,   0, 0), 255);
    delay(350);
    colorWipe(strip.Color(  0,   0,   0, 255), 255);
    delay(350);
}
//END void loop()
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void segLight(int col){
  //use this to light up a segment
  //digit picks which neopixel strip
  //seg calls a segment
  //col is color
  int color[4];

  //color sets
    if (col==0){ //off
      color[0]={0};
      color[1]={0};
      color[2]={0};
      color[3]={0};
    }
    if (col==1){ //red
      color[0]={0};
      color[1]={0};
      color[2]={0};
      color[3]={255};
    }
    if (col==2){ //green
      color[0]={0};
      color[1]={255};
      color[2]={0};
      color[3]={0};
    }
    if (col==3){ //blue
      color[0]={0};
      color[1]={0};
      color[2]={255};
      color[3]={0};
    }
    if (col==4){ //white -- careful with this one, 3x power consumption
      //if 255 is used
      color[0]={100};
      color[1]={100};
      color[2]={100};
      color[3]={0};
    }

      if (col==5){ //amber
      color[0]={0};
      color[1]={255};
      color[2]={210};
      color[3]={105};
    }

    for(int i=0; i<3; i++){
      strip.setPixelColor((i,color[0],color[1],color[2],color[3]),255);
    }
}
//END void segLight()
////////////////////////////////////////////////////////////////////////////////

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(2, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}
