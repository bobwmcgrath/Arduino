// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define LED_COUNT  56
              
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(analogRead(A0)/4); // Set BRIGHTNESS to about 1/5 (max = 255)
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
    digitWrite(0,2); //display it
    strip.show();
    delay(200);
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
  strip.setBrightness(analogRead(A0)/4);
  //use this to light up a segment
  //digit picks which neopixel strip
  //seg calls a segment
  //col is color
  int color[3];

  //color sets
    if (col==0){ //off
      color[0]={0};
      color[1]={0};
      color[2]={0};
    }
    if (col==1){ //red
      color[0]={230};
      color[1]={80};
      color[2]={15};
    }
    if (col==2){ //red
      color[0]={255};
      color[1]={80};
      color[2]={20};
    }
  //sets are 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56
  //seg F
  if(seg==7){
    //light first 8
    for(int i=0; i<8; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
    }
  }
  //seg A
  if(seg==6){
      //light second 8
      for(int i=8; i<16; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
      }
  }
  //seg B
  if(seg==5){
      for(int i=16; i<24; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
      }
  }
  //seg C
  if(seg==4){
      for(int i=24; i<32; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
      }
  }
  //seg D
  if(seg==3){
      for(int i=32; i<40; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
      }
  }
  //seg E
  if(seg==2){
      for(int i=40; i<48; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
      }
  }
  //seg G
  if(seg==1){
      for(int i=48; i<56; i++){
      strip.setPixelColor(i,color[0],color[1],color[2]);
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
