#include "capteur.h"
#include <Math.h>
#include <Arduino.h>
#include "OpticalEncodersLib.h"
#include "espcom.h"
#include "positionLib.h"

int pos = 0;
float angle_cap;
int offset = 0;
int vitesse = 0;
float temps = 1;

Ultrasonic ultrasonicAvant(9, 10); // Trig et Echo
Ultrasonic ultrasonicWall(11, 12); // Trig et Echo

bool ObstacleHere(){
  Serial.println(ultrasonicAvant.Ranging(CM));
  int dist = ultrasonicAvant.Ranging(CM);
  return(dist <= 19);
}


int Wall(){
  return (ultrasonicWall.Ranging(CM));
}

int compteur = 0;
int dist = 0;
void Scan(){
  if(compteur>10){
    dist = Wall();
    compteur = 0;
  }
  compteur++;
  int x = getPositionX();
  int y = getPositionY();
  
  //Serial.println("debut");
  //Serial.println(x);
  //Serial.println(y);
  //Serial.println(roverRotation());
  //Serial.println(roverRotation() * 57.32);
  sendPosRobot(x/5,y/5);
  x = dist * -cos(roverRotation()) + x;
  y = dist * -sin(roverRotation()) + y;
  //Serial.println(x);
  //Serial.println(y);
  sendPoint(x/5,y/5);
}

void majAngle(){
  if(isStraight())
    offset = analogRead(A0);
  else{
    vitesse = analogRead(A0) - offset;
    if(abs(vitesse)>=10)
      pos += vitesse * temps;
      angle_cap = pos * 0.45;
  }
}

float getAngle(){
    return angle_cap;
}

