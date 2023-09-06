#include "gui.h"
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite spr = TFT_eSprite(&tft);
#define WIDTH 536
#define HEIGHT 240

void initScreen() {
  rm67162_init();
  lcd_setRotation(1);
  spr.createSprite(WIDTH, HEIGHT);
  spr.setSwapBytes(1);
}
void updateDiveScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime, float compass, float MOD, float maxDepth) {
  spr.fillSprite(TFT_BLACK);
  //HEADER
  spr.setTextFont(4);
  spr.setTextSize(1);
  spr.setCursor(0, 0);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.println((int)compass);

  spr.setCursor(WIDTH / 2 - 30, 0);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  printFloatAsMinSec(diveTime);

  spr.setCursor(WIDTH - 50, 0);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.println("16C");

  //Depth
  spr.setCursor(WIDTH / 2 - 70, 23);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextSize(1);
  //spr.println("Depth");
  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.setTextFont(4);
  if (depth >= (int)MOD) {
    spr.setTextColor(TFT_RED, TFT_BLACK);
  }
  if (depth >= (int)MOD - 6) {
    spr.setTextColor(TFT_ORANGE, TFT_BLACK);
    if (depth >= (int)MOD) {
      spr.setTextColor(TFT_RED, TFT_BLACK);
    }
    spr.setCursor(WIDTH / 2 - 30, 22);
    spr.print((int)MOD);
  }

  spr.setTextSize(2);
  spr.setCursor(12, 22);
  spr.setTextFont(7);
  if (depth <= 99) {
    spr.print(depth, 1);
  } else {
    spr.print(depth, 0);
  }
  spr.setTextSize(2);
  spr.setTextFont(4);
  spr.setCursor(WIDTH / 2 - 45, HEIGHT / 2 - 40);
  spr.setTextColor(0x7FF, TFT_BLACK);
  spr.println("m");

  //NDL
  spr.setCursor(WIDTH / 2 + 2, HEIGHT / 2 + 1, 2);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextSize(2);
  spr.println("NDL");
  spr.setCursor(WIDTH / 2 + 2, HEIGHT / 2 + 34);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.setTextSize(1);
  spr.setTextFont(7);
  if (NDL >= 0) {
    if (NDL / 60 < 3) {
      spr.setTextColor(TFT_ORANGE, TFT_BLACK);
    }
    spr.print((int)NDL / 60);
    spr.print(":");
  } else {
    spr.setCursor(WIDTH / 2 + 2, HEIGHT / 2 + 1, 2);
    spr.setTextColor(0x07FF, TFT_BLACK);
    spr.setTextSize(2);
    spr.println("Timer:");
    spr.setTextSize(1);
    spr.setCursor(WIDTH / 2 + 2, HEIGHT / 2 + 34);
    spr.setTextColor(TFT_ORANGE, TFT_BLACK);
    spr.setTextFont(7);
    printFloatAsMinSec(currentStopTime);
  }
  //Stop Depth
  spr.setCursor(WIDTH / 2 + 2, 22, 2);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextSize(2);
  spr.println("Stop:");
  spr.setTextSize(1);
  if (NDL < 0) {
    spr.setCursor(WIDTH / 2, 55);
    spr.setTextColor(TFT_ORANGE, TFT_BLACK);
    if (depth < currentStopDepth + 0.5) {
      spr.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    if (depth < currentStopDepth) {
      spr.setTextColor(TFT_RED, TFT_BLACK);
    }
    spr.setTextFont(7);
    spr.print(currentStopDepth, 0);
    spr.setTextFont(4);
    spr.setCursor(WIDTH - 85, 22);
    spr.print((int)TTS / 60 + 1);
    spr.print("min");
    spr.setCursor(WIDTH - 55, HEIGHT / 2 - 40);
    spr.setTextSize(2);
    spr.setTextColor(0x07FF, TFT_BLACK);
    spr.println("m");
  }

  //Footer
  spr.setTextSize(1);
  spr.setCursor(0, HEIGHT - 20);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextFont(4);
  spr.println("21/0->");

  spr.setCursor(WIDTH / 2 - 30, HEIGHT - 20, 2);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextFont(4);
  spr.println("50/0");

  spr.setCursor(WIDTH - 120, HEIGHT - 20, 2);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextFont(4);
  spr.print("max: ");
  spr.print(maxDepth);
  spr.println("m");

  //tft.fillRectHGradient(0, 22, 320, 3, TFT_RED, TFT_GREEN);
  spr.drawFastHLine(0, 22, WIDTH, 0x07FF);
  spr.drawFastHLine(10, HEIGHT / 2, WIDTH - 20, 0x07FF);
  spr.drawFastHLine(0, HEIGHT - 22, WIDTH, 0x07FF);
  spr.drawFastVLine(10, 22, HEIGHT - 44, 0x07FF);
  spr.drawFastVLine(WIDTH / 2, 22, HEIGHT - 44, 0x07FF);
  spr.drawFastVLine(WIDTH - 10, 22, HEIGHT - 44, 0x07FF);

  lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
}
void printFloatAsMinSec(float time) {
  if (time / 60 < 10) {
    spr.print("0");
  }
  if (time / 60 < 1) {
    spr.print("0");
  } else {
    spr.print((int)time / 60);
  }
  spr.print(":");
  if (((int)time % 60) < 10) {
    spr.print("0");
  }
  spr.println((int)time % 60);
}
void print_Serial(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime, float compass, float MOD){
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
  Serial.println(";");
}
void updateSurfaceScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime, float compass, float MOD, float maxDepth){
spr.setCursor(WIDTH / 2 - 70, HEIGHT);
  spr.setTextColor(0x07FF, TFT_BLACK);
  spr.setTextSize(1);
  spr.println("Depth");
    lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
}