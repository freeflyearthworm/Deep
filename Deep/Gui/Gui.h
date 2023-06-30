#ifndef GUI_H
#define GUI_H
#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
void updateDiveScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime);
void initScreen();
#endif