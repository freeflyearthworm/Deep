#include "Buhlmann.h"
#include "gui.h"
#include <Wire.h>

#include "MS5837.h"
MS5837 sensor;

unsigned long clockTick;
float depth = 0;
float diveTime = 0;
float NDL = 5940;
float TTS = 0;
float ceiling = -99;
int currentStopDepth = 0;
float currentStopTime = 0;
float MOD = 0;
float compass = 270;
float maxDepth = 0;
float avgDepth=0;
long timer = 0;
bool dive = false;

void setup(void) {
  Serial.begin(115200);
  initScreen();
  Wire.begin();
  sensor.init();
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  //updateBuhlmann(120, 90);  //testing
}

void loop() {
  clockTick = millis();
  depth = readDepth();
  setMaxAvgDepth(depth);
    if (!dive && depth > 1) {
    startDive();
  }
  if (dive && depth <= 0.5) {
    stopDive();
  }
  diveTime = getDiveTime();
  ceiling = getCeiling();
  NDL = getNDL(depth);
  TTS = getTTS();
  currentStopDepth = getCurrentStopDepth(ceiling);
  currentStopTime = getStopTime(currentStopDepth, currentStopDepth - 3);
  MOD = getMOD();
  print_Serial(depth, diveTime, NDL, TTS, ceiling, currentStopDepth, currentStopTime, compass, MOD);

  if(dive){
    updateDiveScreen(depth, diveTime, NDL, TTS, ceiling, currentStopDepth, currentStopTime, compass, MOD, maxDepth);
  }else{
    updateDiveScreen(depth, diveTime, NDL, TTS, ceiling, currentStopDepth, currentStopTime, compass, MOD, maxDepth);
    updateSurfaceScreen(depth, diveTime, NDL, TTS, ceiling, currentStopDepth, currentStopTime, compass, MOD, maxDepth);
  }
  while ((clockTick + 1000) > millis()) yield();
  updateBuhlmann(depth, 1);  //update every second
  //updateBuhlmann(depth, 10);  //testing
}
float readDepth() {
  if (Serial.available()) {
    float goal = Serial.parseFloat();
    return (goal);
  } else {
    return depth;
  }
  /*
  sensor.read();
  sensor.pressure();
  sensor.altitude();
  sensor.tempreature();
  return (sensor.depth());
  */
}
void startDive() {
  Serial.println("Start Dive");
  timer = millis() + 1000;  //add first clockTick
  dive = true;
  maxDepth=0;
  avgDepth=0;
  //Simulate
  /*updateBuhlmann(depth/2,depth/10*60);
  updateBuhlmann(depth,20*60);*/
}
void stopDive() {
  Serial.println("Stop Dive");
  timer = millis();
  dive = false;
}
long getDiveTime() {
  return (millis() - timer) / 1000;
}
void setMaxAvgDepth(float depth) {
  if (maxDepth < depth) {
    maxDepth = depth;
  }
  avgDepth=(avgDepth*diveTime+depth)/(diveTime+1);
}