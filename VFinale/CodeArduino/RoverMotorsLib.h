/*
 * PROJET FIL ROUGE PARTIE 3
 * AUTEUR : Marc GUEDON
 * ROLE : Librairie pour l'utilisation des moteurs
 */ 

#ifndef __ROVER_MOTORS_LIB__
#define __ROVER_MOTORS_LIB__

#define LEFT_MOTORS_PIN   6
#define RIGHT_MOTORS_PIN  7

//Fonction initialisation moteurs
void initRover();

//Fonctions avancer
void avancer(int vitesse);
void avanceDroite(int vitesse);
void avanceGauche(int vitesse);
void avancerTOR();
void avanceDroiteTOR();

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
