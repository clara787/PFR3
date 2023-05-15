#include "capteur.h"

  Ultrasonic ultrasonicAvant(9, 10); // Trig et Echo

  Ultrasonic ultrasonicWall(12, 13); // Trig et Echo


boolean ObstacleHere(){
  int dist = ultrasonicAvant.Ranging(CM);

  return(dist <= 5);
}
int Wall(){

  return (ultrasonicWall.Ranging(CM));
}
