/*
 * PROJET FIL ROUGE PARTIE 3
 * AUTEUR : Marc GUEDON
 * ROLE : Librairie pour l'utilisation des encodeurs optiques
 */ 

#include "OpticalEncodersLib.h"
#include <Arduino.h>

//Variables modifiées par interruptions
volatile long rightCounter = 0;
volatile long leftCounter = 0;

float rotationAngle = 0;

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

//Fonction getter
long getLeftCounter(){
  long tmp = leftCounter;
  leftCounter = 0;
  return tmp;
}

//Fonction getter
long getRightCounter(){
  long tmp = rightCounter;
  rightCounter =0;
  return tmp;
}

bool isStraight(){
  return (!((leftCounter < 0 && rightCounter > 0) || (leftCounter > 0 && rightCounter < 0)));
}

//Fonction calcul rotation rover
float roverRotation(){
  //Le robot tourne à gauche
  if(leftCounter < 0 && rightCounter > 0){
    float distance = counterToCm(rightCounter);
    rotationAngle -= (distance / ROVER_RADIUS);  //Calcul rotation rover (en radian)
  }

  //Le robot tourne à droite
  else if(leftCounter > 0 && rightCounter < 0){
    float distance = counterToCm(leftCounter);
    rotationAngle += (distance / ROVER_RADIUS); //Calcul rotation rover (en radian)
  }

  return rotationAngle/PI;
}

//Fonction remise à zéro rotation rover
void razAngle(){
  rotationAngle = 0;
}