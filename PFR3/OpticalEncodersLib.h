#ifndef __ENCODERS_LIB__
#define __ENCODERS_LIB__

#define R_CLOCKWISE_PIN           4
#define R_COUNTER_CLOCKWISE_PIN   5
#define L_CLOCKWISE_PIN           6
#define L_COUNTER_CLOCKWISE_PIN   7
//PPR : nombre d'impulsions par révolution
#define PPR                       3000.0
#define WHEEL_DIAM                12.065
#define PI                        3.14
#define ROVER_RADIUS              13.7

//Fonction initialisation encodeurs
void initEncoders();

//Fonction interruption encodeur droite
void rightInterruptFunction();

//Fonction interruption encodeur gauche
void leftInterruptFunction();

//Fonction qui calcul la distance effectuée
float counterToCm(long counter);

//fonction getter
long getLeftCounter();
long getRightCounter();

float roverRotation();

//boolean isTurning();
bool isStraight();

#endif