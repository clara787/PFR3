//################################################################################
//#Auteur: BAFFOGNE Clara                                                        #
//#Fonction: Cette bibliothèque permet de calculer la position du Robot          #
//#                                                                              #
//################################################################################
#include "positionLib.h"
#include "OpticalEncodersLib.h"
#include <Math.h>
#include "capteur.h"
#include <Arduino.h>

//position initiale du robot
float x = 500;
float y = 500;

//fonction Getteur X
float getPositionX(){
    return x;
}

//fonction Getteur Y
float getPositionY(){
    return y;
}

//fonction mise à jour position
void newPosition(){
    //recup distance fonction marc
    long rightDist = getRightCounter();
    long leftDist = getLeftCounter();

    //calcul des distances en fonction des diamètres des roues
    float distRight = counterToCm(rightDist);
    float distLeft = counterToCm(leftDist);
    
    //on récupère la moyenne des deux distances
    float dist = (distRight+distLeft)/2;

    //On applique les formules trigonométriques afin d'avoir les nouvelles coordonnées du robot
    x += dist * -sin(roverRotation()); 
    y += dist * cos(roverRotation());
}

//fonction permettant de remettre à 0 les positions
void razPos(){
  x = 500;
  y = 500;
}