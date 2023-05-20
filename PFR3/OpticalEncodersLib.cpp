#include "OpticalEncodersLib.h"
#include <Arduino.h>

volatile long rightCounter = 0;
volatile long leftCounter = 0;

//Fonction initialisation encodeurs
void initEncoders(){
  pinMode(R_CLOCKWISE_PIN, INPUT);
  pinMode(R_COUNTER_CLOCKWISE_PIN, INPUT);
  pinMode(L_CLOCKWISE_PIN, INPUT);
  pinMode(L_COUNTER_CLOCKWISE_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(R_CLOCKWISE_PIN), rightInterruptFunction, RISING);
  attachInterrupt(digitalPinToInterrupt(L_CLOCKWISE_PIN), leftInterruptFunction, RISING);
}

//Fonction interruption encodeur droite
void rightInterruptFunction(){
  int rightAChannel = digitalRead(R_CLOCKWISE_PIN);
  int rightBChannel = digitalRead(R_COUNTER_CLOCKWISE_PIN);

  if(rightAChannel == rightBChannel){
    rightCounter--;
  }

  else{
    rightCounter++;
  }
}

//Fonction interruption encodeur gauche
void leftInterruptFunction(){
  int leftAChannel = digitalRead(L_CLOCKWISE_PIN);
  int leftBChannel = digitalRead(L_COUNTER_CLOCKWISE_PIN);

  if(leftAChannel == leftBChannel){
    leftCounter++;
  }

  else{
    leftCounter--;
  }
}

//Fonction convertion counter (nombre d'impulsions) -> distance
float counterToCm(long counter){
  float angle = counter / PPR * 360.0; // Calcul angle effectué (en degré)
  return (angle*PI/180) * (WHEEL_DIAM/2); //Calcul distance parcourue (en cm)
}

long getLeftCounter(){
  long tmp = leftCounter;
  leftCounter = 0;
  return tmp;
}

long getRightCounter(){
  long tmp = rightCounter;
  rightCounter =0;
  return tmp;
}

bool isStraight(){
  return (!((leftCounter < 0 && rightCounter > 0) || (leftCounter > 0 && rightCounter < 0)));
}

//Fonction calcul rotation rover
//Renvoie une valeur négative si tourne à gauche      
//        une valeur positive si tourne à droite
//        0 sinon
float rotationAngle = 0;
float roverRotation(){
  //float rotationAngle;

  //Le robot tourne à gauche
  if(leftCounter < 0 && rightCounter > 0){
    float distance = counterToCm(rightCounter);
    rotationAngle -= (distance / ROVER_RADIUS);  //Calcul rotation rover (en degré)
  }

  //Le robot tourne à droite
  else if(leftCounter > 0 && rightCounter < 0){
    float distance = counterToCm(leftCounter);
    rotationAngle += (distance / ROVER_RADIUS); //Calcul rotation rover (en degré)
  }

  //Le robot avance ou recule
  //else{
  //  rotationAngle = 0;
  //}

  return rotationAngle/3;
}

void razAngle(){
  rotationAngle = 0;
}

//Fonction 
/*boolean isTurning(){
  return (leftCounter < (rightCounter-10)) || (leftCounter > (rightCounter+10));
}*/