#include "capteur.h"
#include <Math.h>
#include "OpticalEncodersLib.h"
#include "espcom.h"
#include "LibGyroscope"

int pos = 0;
float angle;

boolean ObstacleHere(){
  Ultrasonic ultrasonicAvant(13, 12); // Trig et Echo
  int dist = ultrasonicAvant.Ranging(CM);

  return(dist <= 25);
}


int Wall(){
  Ultrasonic ultrasonicWall(11, 10); // Trig et Echo
  return (ultrasonicWall.Ranging(CM));
}

void Scan(){
  int dist = Wall();
  int x = getPositionX();
  int y = getPositionY();

  sendPosRobot(x,y);

  x = dist * cos(getAngle()) + x;
  y = dist * sin(getAngle()) + y;

  sendPoint(x,y);
}

void majAngle(){
    if(isStraight())
    offset = analogRead(A0);
   else{
  
  vitesse = analogRead(A0) - offset;
  if(abs(vitesse)>=10)
    pos += vitesse * 0.1;
    angle = pos * 0.45;
   }
}

float getAngle(){
    return angle;
}
