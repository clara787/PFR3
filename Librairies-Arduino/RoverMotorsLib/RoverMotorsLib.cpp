#include <Servo.h>
#include <Arduino.h>
#include "RoverMotorsLib.h"

Servo servoGauche;
Servo servoDroite;

void initRover(){
  servoGauche.attach(LEFT_MOTORS_PIN);
  servoDroite.attach(RIGHT_MOTORS_PIN);
}

//Fonction avancer avec choix vitesse (entre 0 et 255)
void avancer(int vitesse){
  servoGauche.writeMicroseconds(map(vitesse, 0, 255, 1550, 1000));
  servoDroite.writeMicroseconds(map(vitesse, 0, 255, 1550, 1000));
}

//Fonction reculer avec choix vitesse (entre 0 et 255)
void reculer(int vitesse){
  servoGauche.writeMicroseconds(map(vitesse, 0, 255, 1550, 2000));
  servoDroite.writeMicroseconds(map(vitesse, 0, 255, 1550, 2000));
}

//Fonction tourner à gauche (sur place) avec choix vitesse (entre 0 et 255)
void tournerGauche(int vitesse){
  servoDroite.writeMicroseconds(map(vitesse, 0, 255, 1550, 1000));
  servoGauche.writeMicroseconds(map(vitesse, 0, 255, 1550, 2000));
}

//Fonction tourner à droite (sur place) avec choix vitesse (entre 0 et 255)
void tournerDroite(int vitesse){
  servoDroite.writeMicroseconds(map(vitesse, 0, 255, 1550, 2000));
  servoGauche.writeMicroseconds(map(vitesse, 0, 255, 1550, 1000));
}

//Fonction arret
void stop(){
  servoGauche.writeMicroseconds(1550);
  servoDroite.writeMicroseconds(1550);
}

//Fonction avancer Tout Ou Rien
void avancerTOR(){
  avancer(255);
}

//Fonction reculer Tout Ou Rien
void reculerTOR(){
  reculer(255);
}