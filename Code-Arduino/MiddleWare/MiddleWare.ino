
#include "capteur.h"
#include "RoverMotorsLib.h"
#include "espcom.h"
#define rxPin 19
#define txPin 18

int mode = 1;
unsigned char Button;
int distanceMur;
int count =0;
boolean presence;

int X = 0;
int Y = 0;
int angle;


// Set up a new SoftwareSerial object

void setup() {
  // initialize the serial communications:
    //pinMode(rxPin, INPUT);
    //pinMode(txPin, OUTPUT);
    // Set the baud rate for the SoftwareSerial object
    initRover();
}

void loop() {
  // put your main code here, to run repeatedly:
  avancer(255);
  /*distanceMur = Wall();
  presence = ObstacleHere();
  Button = getCharacterESP();
  Serial.println(Button);
  if (Button == 'A') mode = 1;
  if (Button == 'M') mode = 2;
   
  if (count > 2000) {
    sendData();
    count = 0;
  }

  if (mode==1) ModeAuto();
  else if (mode==2) ModeManuel();  

  count++;*/
  
}


void ModeAuto(){

        if((distanceMur<40)&&!presence){
           reculer(100);
        }
        else if(presence && (distanceMur<40)){
            tournerGauche(100);
            delay(1500);
        }
        else if(distanceMur > 40) {
         tournerDroite(100);
         delay(1500);
         reculer(100);
         delay(1500);
        }
}

void ModeManuel(){
    if(Button == 'B') {
      avancerTOR();
      delay(5000);
    }
    else if(Button == 'H') {
      reculerTOR();
      delay(5000);
    }
    else if(Button == 'G') {
      tournerGauche(100);
      delay(5000);
    }
    else if(Button == 'D') {
      tournerDroite(100);
      delay(5000);
    }
    else stop();
    }
void sendData(){}
