#include <Servo.h>

#define LEFT_MOTORS_PIN   2
#define RIGHT_MOTORS_PIN  3

Servo servoGauche;
Servo servoDroite;

void setup() {
  servoGauche.attach(LEFT_MOTORS_PIN);
  servoDroite.attach(RIGHT_MOTORS_PIN);
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