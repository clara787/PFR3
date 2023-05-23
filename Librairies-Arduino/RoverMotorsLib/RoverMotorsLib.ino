#include "RoverMotorsLib.h"

void setup() {
  initRover();
}

void loop() {
  avancer(128);
  delay(500);
  stop();
  delay(500);
  reculer(128);
  delay(500);
  stop();
  delay(500);
  tournerGauche(128);
  delay(500);
  stop();
  delay(500);
  tournerDroite(128);
  delay(500);
  stop();
  delay(500);
}