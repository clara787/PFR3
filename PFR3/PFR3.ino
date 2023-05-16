
#include "capteur.h"
#include "RoverMotorsLib.h"
#include "espcom.h"
#include "positionLib.h"

#define rxPin 19
#define txPin 18

int mode = 2;
int mouvement = 0;
unsigned char Button;
int distanceMur;
int count =0;
boolean presence;

int X = 0;
int Y = 0;
float angle;
unsigned long previousMillis = 0;
unsigned long interval = 1000;

// Set up a new SoftwareSerial object

void setup() {
  // initialize the serial communications:
    //pinMode(rxPin, INPUT);
    //pinMode(txPin, OUTPUT);
    // Set the baud rate for the SoftwareSerial object
    Serial.begin(9600);
    initRover();
    setup_ESPCOM();
}

void loop(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = millis();
    Update();
    Scan();
  }
  // put your main code here, to run repeatedly:
  distanceMur = Wall();
  presence = ObstacleHere();
  Button = getCharacterESP();
  if (Button == 'A') mode = 1;
  if (Button == 'M') mode = 2;
 

  if (mode==1) ModeAuto();
  else if (mode==2) ModeManuel();
  
  if(mouvement == 0) angle = getAngle();
  else if (abs(getAngle() - angle) >=85) mouvement = 0;
  
}


void ModeAuto(){
        if((distanceMur<40)&&!presence && (mouvement ==0){
           avancer(200);
        }
        else if((presence && (distanceMur<40))||(mouvement == 1) ){
            mouvement = 1;
            tournerGauche(200);
        }
        else if((distanceMur > 40)||(mouvement == 2)) {
         tournerDroite(200);
         mouvement = 2;
        }
        else avancer(200);
}

int mode_Manuel=0;
void ModeManuel(){
    if(Button == 'B')mode_Manuel = 4;
    if(Button == 'H')mode_Manuel = 3;
    if(Button == 'G')mode_Manuel = 2;
    if(Button == 'D')mode_Manuel = 1;
    if(Button == 'S')mode_Manuel = 0;

    if(mode_Manuel == 4) {
      reculer(200);
    }
    else if(mode_Manuel == 3){
      avancer(200);
    }
    else if(mode_Manuel == 2) {
      tournerGauche(200);
    }
    else if(mode_Manuel == 1){
      tournerDroite(200);
    }
    else stop();
}

void Update(){
  newPosition();
  majAngle();
}
