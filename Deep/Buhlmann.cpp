#include "Buhlmann.h"
float t05N2[16] = { 4, 8, 12.5, 18.5, 27, 38.3, 54.3, 77, 109, 146, 187, 239, 305, 390, 498, 635 };
float aN2[16] = { 1.2600, 1.0001, 0.8618, 0.7563, 0.6667, 0.5934, 0.5282, 0.4702, 0.4188, 0.3799, 0.3498, 0.3223, 0.2972, 0.2738, 0.2524, 0.2327 };
float bN2[16] = { 0.5050, 0.6514, 0.7222, 0.7725, 0.8125, 0.8434, 0.8693, 0.8910, 0.9092, 0.9222, 0.9319, 0.9403, 0.9477, 0.9544, 0.9602, 0.9653 };
float Po[16] = { 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7400, 0.7200, 0.7400, 0.7400 };  //Po=(Pamb-PH2O)*N2=(10-0.627)*0.79 //0.72?????
float backup_Po[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

float surfacePressure = 1;  //1 Bar surface Pressure
float N2 = 0.79;            //78% Nitrogen

struct gas{
  float o2;
  float partPressure;
  float he;
  bool isActive;
};
gas backGas = {0.21,1.4,0,true};
gas DecoGas1 = {0.5,1.6,0,false};
gas currentGas = backGas;

void updateBuhlmann(float depth, int inc) {
  float Pamb = depth * 0.1 + surfacePressure;  //depth pressure
  float Pi = (Pamb - 0.063) * N2;              //inertgas pressure
  for (int i = 0; i < 16; ++i) {
    Po[i] = Po[i] + (Pi - Po[i]) * (1 - pow(2, (-inc / t05N2[i] / 60)));  //Haldane equation //60s //compartment pressure
  }
}
float getCeiling() {
  float ceilingPressure = -999;
  for (int i = 0; i < 16; ++i) {
    if (ceilingPressure < (Po[i] - aN2[i]) * bN2[i]) {
      ceilingPressure = (Po[i] - aN2[i]) * bN2[i];
    }
  }
  float ceilingDepth = pressureToDepth(ceilingPressure);
  if (ceilingDepth < 0) {
    return 0;
  } else {
    return ceilingDepth;
  }
}
float getNDL(float depth) {
  float Pamb = depth * 0.1 + surfacePressure;
  float Pi = (Pamb - 0.0627) * N2;  //Pi/10
  float t;
  float NDL = 5940;
  for (int i = 0; i < 16; ++i) {
    float Mo = ((surfacePressure / bN2[i]) + aN2[i]);  //Mo Surface Pressure or 1??????
    if (Po[i] < Mo) {
      if (Mo < Pi) {
        float k = 0.693 / t05N2[i] / 60;  //ln(29/half-time)
        t = (-1.0 / k) * log((Pi - Mo) / (Pi - Po[i]));
        if (t < NDL) {
        NDL = t;
        }
      }
    } else {
      NDL = -1;
      break;
    }
  }
  return NDL;
}
float getTTS() {
  float TTS = 0;
  for (int i = 0; i < 16; i++) {//safe compartments for ascend simulation
    backup_Po[i] = Po[i];
  }
  int stopDepth = getCurrentStopDepth(getCeiling());
  while (stopDepth > 0) {
    float stopTime = 0;
    stopTime = getStopTime(stopDepth, stopDepth - 3);
    Serial.print(" ");
    Serial.print(stopTime);
    Serial.print("s@");
    Serial.print(stopDepth);
    Serial.print("m ->");
    TTS = TTS + stopTime;
    updateBuhlmann(stopDepth, stopTime);
    stopDepth = stopDepth - 3;
  }
  for (int i = 0; i < 16; i++) {//
    Po[i] = backup_Po[i];
  }
  Serial.println(";");
  return TTS;
}
float depthToPressure(float depth) {
  return (depth * 10 + surfacePressure);
}
float pressureToDepth(float pressure) {
  return (pressure - surfacePressure) * 10;
}
int getCurrentStopDepth(float ceiling) {
  if(ceiling==0){
    return 0;
  }
  int stopDepth = ceiling + 1;
  while (stopDepth % 3 != 0) {
    stopDepth++;
  }
  return stopDepth;
}

float getStopTime(float depth, float depth2) {
  float Pamb = depth * 0.1 + surfacePressure;  //Pamb/10
  float Pi = (Pamb - 0.0627) * N2;             //Pi/10
  float t = 0;
  float stopTime = 0;
  for (int i = 0; i < 16; ++i) {
    float Mo = (((depth2 * 0.1 + surfacePressure) / bN2[i]) + aN2[i]);  //Mo Surface Pressure or 1??????
    if (Po[i] > Mo) {
      if (Mo > Pi) {
        if (Po[i] > Pi) {
          float k = 0.693 / t05N2[i] / 60;  //ln(29/half-time)
          t = (-1.0 / k) * log((Pi - Mo) / (Pi - Po[i]));
          if (t > stopTime) {
            stopTime = t;
          }
        }
      }
    }
  }
  return stopTime;
}
float getMOD(){
return(10 * ((currentGas.partPressure / currentGas.o2) - surfacePressure));
}