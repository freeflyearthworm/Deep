#ifndef BUHLMANN_H
#define BUHLMANN_H
#include "Arduino.h"
#include <math.h>

void startDive();
void updateBuhlmann(float depth, int inc);
float getCeiling();
long  getDiveTime();
float getNDL(float depth);
float getStopTime(float depth,float depth2);
float getTTS();
void stopDive();
float haldaneTime(int compartment, float surfacePressure, float N2, float depth, float Mo);
float pressureToDepth(float pressure);
float depthToPressure(float depth);
int getCurrentStopDepth(float ceiling);
#endif