#include "Gui.h"

void initScreen(){

    tft.init();
  tft.setRotation(1);

}
void updateDiveScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime) {
  tft.fillScreen(TFT_BLACK);
  //Header
    //Compass
  tft.setCursor(0, 0, 2);
  tft.setCursor(0, 14);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(2);
  //tft.print(, 1);
  tft.println("270°");
  //Time
  tft.setCursor(160-15, 0, 2);
  tft.setCursor(0, 14);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(2);
  //tft.print(, 1);
  tft.println("00:00");
  //Temperature
  tft.setCursor(0, 0, 2);
  tft.setCursor(0, 14);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(2);
  //tft.print(, 1);
  tft.println("270°");

  //Depth
  /*tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Depth:");
  tft.setCursor(0, 14);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(6);
  tft.print(depth, 1);
  tft.println("m");

  tft.setCursor(0, 60, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("NDL/TTS");
  tft.setCursor(0, 74);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(6);
  if (NDL >= 0) {
    tft.print((int)NDL / 60, 0);
    tft.setTextFont(4);
    tft.print(((int)NDL % 60), 0);
  } else {
    tft.print("0");
    tft.print((int)TTS / 60, 0);
    tft.setTextFont(4);
    tft.print((int)TTS % 60, 0);
    tft.setCursor(90, 76, 2);
    tft.setTextFont(6);
    tft.print((int)currentStopTime / 60, 0);
    tft.setTextFont(4);
    tft.print(((int)currentStopTime % 60), 0);
  }

  tft.setCursor(320 / 2, 0, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Dive/Surface Time:");
  tft.setCursor(160, 14);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(6);
  tft.print((int)diveTime / 60, 0);
  tft.setTextFont(4);
  tft.print(((int)diveTime % 60), 0);

  tft.setCursor(320 / 2, 60, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Stop:");
  tft.setCursor(160, 74);
  tft.setTextColor(0x07FF, TFT_BLACK);
  tft.setTextFont(6);
  tft.print(currentStopDepth, 0);
  tft.print("m");*/

  tft.drawFastHLine(0, 60, 320, TFT_WHITE);
  tft.drawFastHLine(0, 120, 320, TFT_WHITE);
  tft.drawFastVLine(320 / 2, 0, 120, TFT_WHITE);
}
