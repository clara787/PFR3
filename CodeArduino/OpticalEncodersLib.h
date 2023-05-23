/*
 * PROJET FIL ROUGE PARTIE 3
 * AUTEUR : Marc GUEDON
 * ROLE : Librairie pour l'utilisation des encodeurs optiques
 */ 

#ifndef __ENCODERS_LIB__
#define __ENCODERS_LIB__

#define R_CLOCKWISE_PIN           2
#define R_COUNTER_CLOCKWISE_PIN   4
#define L_CLOCKWISE_PIN           3
#define L_COUNTER_CLOCKWISE_PIN   5

#define PPR                       3000.0 //PPR : nombre d'impulsions par révolution
#define WHEEL_DIAM                12.065
#define PI                        3.14
#define ROVER_RADIUS              13.7

//Fonction initialisation encodeurs
void initEncoders();

//Fonction interruption encodeur droite
void rightInterruptFunction();

//Fonction interruption encodeur gauche
void leftInterruptFunction();

//Fonction calcul distance effectuée
float counterToCm(long counter);

//Fonctions getter
long getLeftCounter();
long getRightCounter();

bool isStraight();

//Fonction calcul rotation rover
float roverRotation();

//Fonction remise à zéro rotation rover
void razAngle();

#endif