#include "OpticalEncodersLib.h"

void setup() {
  initEncoders();

  Serial.begin(9600);
  Serial.println("Prêt");
}

void loop() {
  noInterrupts();
  long rightCounter = getRightCounter();
  long leftCounter = getLeftCounter();
  interrupts();

  Serial.print("Compteur encodeur droite : ");
  Serial.println(rightCounter);
  Serial.print("Distance (cm) : ");
  Serial.println(counterToCm(rightCounter));

  
  Serial.print("Compteur encodeur gauche : ");
  Serial.println(leftCounter);
  Serial.print("Distance (cm) : ");
  Serial.println(counterToCm(leftCounter));
  delay(1000);
}