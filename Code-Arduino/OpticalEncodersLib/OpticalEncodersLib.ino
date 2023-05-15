#include "OpticalEncodersLib.h"

void setup() {
  void initEncoders();

  Serial.begin(9600);
  Serial.println("Prêt");
}

void loop() {
  float rightCounter = getRightCounter();
  Serial.print("Compteur encodeur droite : ");
  Serial.println(rightCounter);
  Serial.print("Distance (cm) : ");
  Serial.println(counterToCm(rightCounter));
  delay(1000);

  //majPosition(20, 20);
}