#include <SPI.h>
#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip
#include "Buhlmann.h"
//#include "Gui.h"
unsigned long clockTick;
float depth = 0;
float diveTime = 0;
float NDL = 5940;
float TTS = 0;
float ceiling = -99;
int currentStopDepth = 0;
float currentStopTime = 0;
float MOD=0;
float compass=270;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


void setup(void) {
  //initScreen();
  tft.init();
  tft.setRotation(1);
  Serial.begin(115200);
  updateBuhlmann(120, 90);  //testing
}

void loop() {
  clockTick = millis();
  depth = readDepth();
  diveTime = getDiveTime();
  ceiling = getCeiling();
  NDL = getNDL(depth);
  TTS = getTTS();
  currentStopDepth = getCurrentStopDepth(ceiling);
  currentStopTime = getStopTime(currentStopDepth, currentStopDepth - 3);
  MOD=10*((1.6/0.21)-1);

  Serial.print("Depth: ");
  Serial.print(depth);
  Serial.print(" Divetime:");
  Serial.print(diveTime);
  Serial.print(" Ceiling: ");
  Serial.print(ceiling);
  Serial.print(" NDL:");
  Serial.print(NDL);
  Serial.print(" TTS:");
  Serial.print(TTS);
  //Serial.println(":");
  //Serial.print((clockTick + 1000) - millis());
  Serial.println(";");
  updateDiveScreen(depth, diveTime, NDL, TTS, ceiling, currentStopDepth, currentStopTime);
  while ((clockTick + 1000) > millis()) yield();
  updateBuhlmann(depth, 1);  //update every second
}
float readDepth() {
  if (Serial.available()) {
    String weight;
    float goal;
    goal = Serial.parseFloat();
    return (goal);
  } else {
    return depth;
  }
}
void updateDiveScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime) {
  tft.fillScreen(TFT_BLACK);
  //HEADER
  tft.setCursor(0, 0, 1);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(4);
  tft.println((int)compass);

  tft.setCursor(160 - 30, 0, 2);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(4);
  printFloatAsMinSec(diveTime);

  tft.setCursor(320 - 50, 0, 2);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(4);
  tft.println("16C");

  //Depth
  tft.setCursor(12, 22, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Depth");
  tft.setTextColor(0x07FF, TFT_BLACK);
    tft.setTextFont(4);
  if(depth>=(int)MOD){
     tft.setTextColor(TFT_RED, TFT_BLACK);
  }
  if(depth>=(int)MOD-6){
       tft.setTextColor(TFT_ORANGE, TFT_BLACK);
         if(depth>=(int)MOD){
     tft.setTextColor(TFT_RED, TFT_BLACK);
  }
   tft.setCursor(160 - 30, 22);
       tft.print((int)MOD);
  }
   tft.setCursor(12, 36,1);
  tft.setTextFont(7);
  if (depth <= 99) {
    tft.print(depth, 1);
  } else {
    tft.print(depth, 0);
  }
  tft.setTextFont(4);
  tft.setCursor(160 - 25, 170 / 2 - 20);
  tft.println("m");

  //NDL
 tft.setCursor(160+ 2, 170 / 2 + 1, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("NDL");
  tft.setCursor(160+2, 170 / 2 + 14);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(7);
  if (NDL >= 0) {
    if(NDL/60<3){
      tft.setTextColor(TFT_ORANGE,TFT_BLACK);
    }
    tft.print((int)NDL/60);
    tft.print(":");
  }
  else{
  tft.setCursor(160+2, 170 / 2 + 1, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("StopTime:");
  tft.setCursor(160+2, 170 / 2 + 14);
  tft.setTextColor(TFT_ORANGE,TFT_BLACK);
  tft.setTextFont(7);
  printFloatAsMinSec(currentStopTime);
  }
  //Stop Depth
  tft.setCursor(320 / 2+2, 22, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Ceiling");
  if(NDL<0){
  tft.setCursor(190, 36);
  tft.setTextColor(TFT_ORANGE,TFT_BLACK);
      if(depth<currentStopDepth+0.5){
     tft.setTextColor(TFT_GREEN, TFT_BLACK);
      }
      if(depth<currentStopDepth){
     tft.setTextColor(TFT_RED, TFT_BLACK);
      }
  tft.setTextFont(7);
  tft.print(currentStopDepth, 0);
  tft.setTextFont(4);
  tft.setCursor(320 - 35, 22);
  tft.print((int)TTS/60+1);
  tft.print(":");
  tft.setCursor(320 - 40, 170 / 2 - 20);
  tft.println("m");
  }
  
  //Footer
  tft.setCursor(0, 150, 1);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(4);
  tft.println("21/0->");

  tft.setCursor(160 - 30, 150, 2);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(4);
  tft.println("50/0");

  tft.setCursor(320 - 90, 150, 2);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(4);
  tft.println("max:77m");

  //tft.fillRectHGradient(0, 22, 320, 3, TFT_RED, TFT_GREEN);
  tft.drawFastHLine(0, 22, 320, TFT_WHITE);
  tft.drawFastHLine(10, 170 / 2, 320 - 10, TFT_WHITE);
  tft.drawFastHLine(0, 148, 320, TFT_WHITE);
  tft.drawFastVLine(10, 22, 126, TFT_WHITE);
  tft.drawFastVLine(320 / 2, 22, 126, TFT_WHITE);
  tft.drawFastVLine(320 - 10, 22, 126, TFT_WHITE);
}
void printFloatAsMinSec(float time) {
  if (time / 60 < 10) {
    tft.print("0");
  }
    if (time / 60 < 1) {
    tft.print("0");
  }
  else{
    tft.print((int)time/60);
  }
  tft.print(":");
  if (((int)time % 60) < 10) {
    tft.print("0");
  }
  tft.println((int)time % 60);
}