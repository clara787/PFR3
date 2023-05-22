
#include "capteur.h"
#include "RoverMotorsLib.h"
#include "espcom.h"
#include "positionLib.h"
#include "OpticalEncodersLib.h"
#include "positionLib.h"


int mode = 1;
int modeTrace = 0;
unsigned char Button;
int distanceMur;
int count =0;
boolean presence;
boolean presencePre;

int X = 0;
int Y = 0;
float angle;
unsigned long previousMillis = 0;
unsigned long interval = 100;

// Set up a new SoftwareSerial object

void setup() {
    // initialize the serial communications:
    // Set the baud rate for the SoftwareSerial object
    Serial.begin(9600);
    initRover();
    setup_ESPCOM();
    initEncoders();
}

void loop(){
  //Envoie les coordonnées du Robot à l'ESP périodiquement
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = millis();
    Scan();
    Update();
  }

  if(mode==1){
    distanceMur = Wall();
    presencePre = presence;
    presence = ObstacleHere();
  }
  Button = getCharacterESP();
  //Serial.println(Button);
  if (Button == 'A') mode = 1;
  if (Button == 'M'){
    mode = 2;
    razAngle();
    razPos();
  }
  if (mode==1) ModeAuto();
  else if (mode==2) ModeManuel();
  
}

// Fonction du mode automatique du robot
void ModeAuto(){
  //if(modeTrace == 0 && (getPositionX()>510 || getPositionX()<490) && (getPositionY()>510 || getPositionY()<490))modeTrace=1;
  //if(modeTrace == 1 && (getPositionX()<510 && getPositionX()>490) && (getPositionY()<510 || getPositionY()>490)){
  //  modeTrace = 0;
  //  mode = 2;
  //}

  // Avance en tournant légèrement vers la gauche en présence d'un mur
  if((distanceMur<=18)&& !(presence&&presencePre)){
    avanceGauche(162);
  }

  //Tourne à gauche lorsqu'un obstacle est en face
  if(presence&&presencePre){
    angle = roverRotation();
    while(abs(roverRotation()-angle)<=95){
    tournerGauche(175);
    }
  }
   // Avance en tournant vers la droite lorsque le mur est un peu éloigné
  if (distanceMur>=18 && distanceMur<=25){
    avanceDroite(162);
  }
  else if (distanceMur>=25){
    avanceDroiteTOR();
  }
}

int mode_Manuel=0;
// Le mode manuel reçoit une direction de l'ESP et le robot se déplace en fonction du signal reçu
void ModeManuel(){
    if(Button == 'B')mode_Manuel = 4;
    if(Button == 'H')mode_Manuel = 3;
    if(Button == 'G')mode_Manuel = 2;
    if(Button == 'D')mode_Manuel = 1;
    if(Button == 'S')mode_Manuel = 0;

    if(mode_Manuel == 4) {
      reculer(150);
    }
    else if(mode_Manuel == 3){
      avancer(150);
    }
    else if(mode_Manuel == 2) {
      tournerGauche(150);
    }
    else if(mode_Manuel == 1){
      tournerDroite(150);
    }
    else stop();
}

void Update(){
  newPosition();
  majAngle();
}
