#ifndef GUI_H
#define GUI_H
#include "Arduino.h"
#include <math.h>
#include <SPI.h>
#include "rm67162.h"
#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip

#if ARDUINO_USB_CDC_ON_BOOT != 1
#warning "If you need to monitor printed data, be sure to set USB CDC On boot to ENABLE, otherwise you will not see any data in the serial monitor"
#endif
#ifndef BOARD_HAS_PSRAM
#error "Detected that PSRAM is not turned on. Please set PSRAM to OPI PSRAM in ArduinoIDE"
#endif




void initScreen();
void printFloatAsMinSec(float time);
void updateDiveScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime, float compass, float MOD, float maxDepth);
void updateSurfaceScreen(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime, float compass, float MOD, float maxDepth);
void print_Serial(float depth, float diveTime, float NDL, float TTS, float ceiling, int currentStopDepth, float currentStopTime, float compass, float MOD);
#endif