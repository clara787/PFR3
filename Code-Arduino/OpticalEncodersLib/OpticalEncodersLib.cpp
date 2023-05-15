#include "OpticalEncodersLib.h"
#include <Arduino.h>

volatile float rightCounter = 0;
volatile float leftCounter = 0;

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
    leftCounter--;
  }

  else{
    leftCounter++;
  }
}

float counterToCm(int counter){
  float angle = counter / PPR * 360.0; // Calcul angle effectué
  return (angle*PI/180) * (WHEEL_DIAM/2); //Calcul distance parcourue
}

float getLeftCounter(){
  return leftCounter;
}

float getRightCounter(){
  return rightCounter;
}