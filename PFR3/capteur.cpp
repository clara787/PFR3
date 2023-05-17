#include "capteur.h"
#include <Math.h>
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

boolean ObstacleHere(){
  int dist = ultrasonicAvant.Ranging(CM);
  return(dist <= 19);
}


int Wall(){
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
    offset = analogRead(A5);
  else{
  
    vitesse = analogRead(A5) - offset;
    if(abs(vitesse)>=10)
      pos += vitesse * temps;
      angle_cap = pos * 0.45;
  }
}

float getAngle(){
    return angle_cap + 90;
}
