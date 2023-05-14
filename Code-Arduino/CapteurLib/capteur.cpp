#include "capteur.h"
#include <Math.h>
#include "OpticalEncodersLib.h"
#include "espcom.h"
#include "LibGyroscope"

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
