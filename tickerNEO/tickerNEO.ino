// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define LED_COUNT  14
#define BRIGHTNESS 255
              
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
    digitWrite(0,2); //display it
    strip.show();
    delay(300);
    digitWrite(0,1); //display it
    strip.show();
    delay(50);
    digitWrite(0,2); //display it
    strip.show();
    delay(1000);
    digitWrite(0,1); //display it
    strip.show();
    delay(50);
    digitWrite(0,2); //display it
    strip.show();
    delay(50);
    digitWrite(0,1); //display it
    strip.show();
    delay(50);
   for (int num=9;num>0;num--){
    digitWrite(num,2); //display it
    strip.show();
    delay(85);
    }
    digitWrite(0,2); //display it
    strip.show();
    delay(1500);
   for (int num=9;num>0;num--){
    digitWrite(num,2); //display it
    strip.show();
    delay(85);
    }
}

////////////////////////////////////////////////////////////////////////////////
void digitWrite(int val, int col){
  //use this to light up a digit
  //digit is which digit panel one (right to left, 0 indexed)
  //val is the character value to set on the digit
  //col is the predefined color to use, R,G,B or W
  //example:
  //        digitWrite(0, 4, 2);
  //would set the first digit
  //on the right to a "4" in green.

/*
// Letters are the standard segment naming, as seen from the front,
// numbers are based upon the wiring sequence

          A 2
     ----------
    |          |
    |          |
F 1 |          | B 3
    |          |
    |     G 7  |
     ----------
    |          |
    |          |
E 6 |          | C 4
    |          |
    |     D 5  |
     ----------    dp 8

*/
//these are the digit panel character value definitions,
//if color argument is a 0, the segment is off
  if (val==0){
    //segments F,A,B,C,D,E,G, dp
    segLight(1,col);
    segLight(2,col);
    segLight(3,col);
    segLight(4,col);
    segLight(5,col);
    segLight(6,col);
    segLight(7,0);
    //segLight(digit,8,col);
  }
  if (val==1){
    segLight(1,0);
    segLight(2,0);
    segLight(3,col);
    segLight(4,col);
    segLight(5,0);
    segLight(6,0);
    segLight(7,0);
    //segLight(digit,8,col);
  }
  if (val==2){
    segLight(1,0);
    segLight(2,col);
    segLight(3,col);
    segLight(4,0);
    segLight(5,col);
    segLight(6,col);
    segLight(7,col);
    //segLight(digit,8,col);
  }
  if (val==3){
    segLight(1,0);
    segLight(2,col);
    segLight(3,col);
    segLight(4,col);
    segLight(5,col);
    segLight(6,0);
    segLight(7,col);
    //segLight(digit,8,col);
  }
  if (val==4){
    segLight(1,col);
    segLight(2,0);
    segLight(3,col);
    segLight(4,col);
    segLight(5,0);
    segLight(6,0);
    segLight(7,col);
    //segLight(digit,8,col);
  }
  if (val==5){
    segLight(1,col);
    segLight(2,col);
    segLight(3,0);
    segLight(4,col);
    segLight(5,col);
    segLight(6,0);
    segLight(7,col);
    //segLight(digit,8,col);
  }
  if (val==6){
    segLight(1,col);
    segLight(2,col);
    segLight(3,0);
    segLight(4,col);
    segLight(5,col);
    segLight(6,col);
    segLight(7,col);
    //segLight(digit,8,col);
  }
  if (val==7){
    segLight(1,0);
    segLight(2,col);
    segLight(3,col);
    segLight(4,col);
    segLight(5,0);
    segLight(6,0);
    segLight(7,0);
    //segLight(digit,8,col);
  }
  if (val==8){
    segLight(1,col);
    segLight(2,col);
    segLight(3,col);
    segLight(4,col);
    segLight(5,col);
    segLight(6,col);
    segLight(7,col);
    //segLight(digit,8,col);
  }
  if (val==9){
    segLight(1,col);
    segLight(2,col);
    segLight(3,col);
    segLight(4,col);
    segLight(5,col);
    segLight(6,0);
    segLight(7,col);
    //segLight(digit,8,col);
  }
}

////////////////////////////////////////////////////////////////////////////////
void segLight(int seg, int col){
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
      color[0]={200};
      color[1]={5};
      color[2]={0};
      color[3]={210};
    }
    if (col==2){ //red
      color[0]={235};
      color[1]={5};
      color[2]={0};
      color[3]={255};
    }
  //sets are 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56
  //seg F
  if(seg==7){
    //light first 8
    for(int i=0; i<2; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
    }
  }
  //seg A
  if(seg==6){
      //light second 8
      for(int i=2; i<4; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
      }
  }
  //seg B
  if(seg==5){
      for(int i=4; i<6; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
      }
  }
  //seg C
  if(seg==4){
      for(int i=6; i<8; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
      }
  }
  //seg D
  if(seg==3){
      for(int i=8; i<10; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
      }
  }
  //seg E
  if(seg==2){
      for(int i=10; i<12; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
      }
  }
  //seg G
  if(seg==1){
      for(int i=12; i<14; i++){
      strip.setPixelColor(i,color[0],color[1],color[2],color[3]);
      }
  }
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
