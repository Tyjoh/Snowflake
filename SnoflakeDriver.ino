#include <SoftPWM.h>

#define DELAY 100
#define NUM_LEDS 18
#define MAX_PWM 200
#define NUM_BRANCHES 12
#define NUM_LAYERS 3
#define NUM_PER_LAYER 6

//for defining different groups of leds for easier control
struct LedGroup {
  byte numLeds;
  uint8_t leds[6];
};

//list of all leds, ordered from bottom center LED clockwise around the
//snowflake
uint8_t leds[NUM_LEDS] = {3,2,6,7,8,9,10,11,12,13,A0,A1,A2,A3,A4,A5,4,5};

//the two different lists of LED groups
LedGroup branches[NUM_BRANCHES];
LedGroup layers[NUM_LAYERS];

int stepNum = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //initialize the branch groups
  branches[0].numLeds = 1;
  branches[0].leds[0] = 3;
  
  branches[1].numLeds = 2;
  branches[1].leds[0] = 2;
  branches[1].leds[1] = 6;
  
  branches[2].numLeds = 1;
  branches[2].leds[0] = 7;
  
  branches[3].numLeds = 2;
  branches[3].leds[0] = 8;
  branches[3].leds[1] = 9;

  branches[4].numLeds = 1;
  branches[4].leds[0] = 10;
  
  branches[5].numLeds = 2;
  branches[5].leds[0] = 11;
  branches[5].leds[1] = 12;

  branches[6].numLeds = 1;
  branches[6].leds[0] = 13;
  
  branches[7].numLeds = 2;
  branches[7].leds[0] = A0;
  branches[7].leds[1] = A1;

  branches[8].numLeds = 1;
  branches[8].leds[0] = A2;
  
  branches[9].numLeds = 2;
  branches[9].leds[0] = A3;
  branches[9].leds[1] = A4;

  branches[10].numLeds = 1;
  branches[10].leds[0] = A5;
  
  branches[11].numLeds = 2;
  branches[11].leds[0] = 4;
  branches[11].leds[1] = 5;

  //initialize the layer groups
  layers[0].numLeds = 6;
  layers[0].leds[0] = 3;
  layers[0].leds[1] = 7;
  layers[0].leds[2] = 10;
  layers[0].leds[3] = 13;
  layers[0].leds[4] = A2;
  layers[0].leds[5] = A5;

  layers[1].numLeds = 6;
  layers[1].leds[0] = 6;
  layers[1].leds[1] = 9;
  layers[1].leds[2] = 11;
  layers[1].leds[3] = A1;
  layers[1].leds[4] = A4;
  layers[1].leds[5] = 5;
  
  layers[2].numLeds = 6;
  layers[2].leds[0] = 2;
  layers[2].leds[1] = 8;
  layers[2].leds[2] = 12;
  layers[2].leds[3] = A0;
  layers[2].leds[4] = A3;
  layers[2].leds[5] = 4;

  //setup sofware pwm
  SoftPWMBegin();

  for(int i = 0; i < NUM_LEDS; i++){
    SoftPWMSet(leds[i], 0);
  }
  
}

void loop() {
  // animation sequencing
  setAll(15);
  fullSyncPulse(3);

  setAll(15);
  branchRotate(3, MAX_PWM, 15, 2);

  setAll(15);
  branchRotate(3, MAX_PWM, 15, 3);
  
  setAll(15);
  linearBounce(4, MAX_PWM, 15);

  setAll(0);
  alternateLayers(1, MAX_PWM, 15, 800);
  alternateLayers(1, MAX_PWM, 15, 700);
  alternateLayers(1, MAX_PWM, 15, 600);
  alternateLayers(1, MAX_PWM, 15, 500);
  alternateLayers(1, MAX_PWM, 15, 400);
  alternateLayers(1, MAX_PWM, 15, 300);
  alternateLayers(1, MAX_PWM, 15, 400);
  alternateLayers(1, MAX_PWM, 15, 500);
  alternateLayers(1, MAX_PWM, 15, 600);
  alternateLayers(1, MAX_PWM, 15, 700);
  alternateLayers(1, MAX_PWM, 15, 800);

}

void fullSyncPulse(int N){

  for(int n = 0; n < N; n++){
    SoftPWMSetFadeTime(ALL, 3000, 3000);
    
    for(int i = 0; i < NUM_LEDS; i++){
      SetLed(i, MAX_PWM);
    }
  
    delay(3000);
  
    for(int i = 0; i < NUM_LEDS; i++){
      SetLed(i, 25);
    }
  
    delay(3000);
  }
}

void linearBounce(int N, int fg, int bg){
  
  SoftPWMSetFadeTime(ALL, 100, 400);
  
  for(int n = 0; n < N; n++){
    for (int i = 1; i < NUM_LEDS - 1; i++)
    {
      SetLed(i+1, fg);
      SetLed(i, bg);
      delay(DELAY);//100
    }
    
    delay(400);//400
    
    for (int i = NUM_LEDS - 1; i > 1; i--)
    {
      SetLed(i - 1, fg);
      SetLed(i, bg);
      delay(DELAY);
    }
  
    delay(400);//400
  }
}

void branchRotate(int N, int fg, int bg, int num){

  SoftPWMSetFadeTime(ALL, (bg * 10) / num, (fg * 10) / num);

  int branchOffset = NUM_BRANCHES / num;

  for(int n = 0; n < N * 12; n++){
    
    for(int i = 0; i < num; i++){
      setBranch((i*branchOffset + n) % NUM_BRANCHES, fg);
    }
    
    delay(250);

    setAll(bg);
  }
}

void alternateLayers(int N, int fg, int bg, int d){
  SoftPWMSetFadeTime(ALL, d + 200, d + 200);

  for(int n = 0; n < N; n++){
    for(int i = 0; i < NUM_LAYERS; i++){
      setLayer(i, fg);
      delay(d);
      setLayer(i, bg);
    }
  }
  
}

void setAll(int val){
  SoftPWMSetFadeTime(ALL, 1000, 1000);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    SetLed(i, val);
  }
}

void setLayer(int layerId, int val){
  LedGroup layer = layers[layerId];
  
  for(int i = 0; i < layer.numLeds; i++){
    SoftPWMSet(layer.leds[i], val);
  }
}

void setBranch(int branchId, int val){
  SoftPWMSet(branches[branchId].leds[0], val);
  
  if(branches[branchId].numLeds == 2)
    SoftPWMSet(branches[branchId].leds[1], val);
}

void SetLed(int i, int val){
  SoftPWMSet(leds[i], val);
}
