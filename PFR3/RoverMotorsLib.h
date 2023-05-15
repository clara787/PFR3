#ifndef __ROVER_MOTORS_LIB__
#define __ROVER_MOTORS_LIB__

#define LEFT_MOTORS_PIN   2
#define RIGHT_MOTORS_PIN  3

//Fonction initialisation
void initRover();

//Fonctions avancer
void avancer(int vitesse);
void avancerTOR();

//Fonctions reculer
void reculer(int vitesse);
void reculerTOR();

//Fonction tourner à gauche
void tournerGauche(int vitesse);

//Fonction tourner à droite
void tournerDroite(int vitesse);

//Fonction arret
void stop();

#endif