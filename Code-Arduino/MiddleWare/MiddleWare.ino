#include "RoverMotorsLib.h"

#include "capteur.h"

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
    // Set the baud rate for the SoftwareSerial object
    initRover();
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  distanceMur = Wall();
  presence = ObstacleHere();
  if (Button == 'A') mode = 1;
  if (Button == 'M') mode = 2;
   
  if (count > 2000) {
    count = 0;
  }

  if (mode==1) ModeAuto();
  else if (mode==2) ModeManuel(); 
  Serial.println(distanceMur);

  count++;

  
}


void ModeAuto(){

        if((distanceMur<40)&&!presence){
           reculer(100);
           Serial.println("avance");
        }
        else if(presence){
            tournerGauche(100);
            delay(1500);
            Serial.println("Gauche");
        }
        else if(distanceMur > 40) {
         tournerDroite(100);
         delay(1500);
         reculer(100);
         delay(1500);
         Serial.println("Droite");
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
