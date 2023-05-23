//################################################################################
//#Auteur: SAIDI Rahim                                                           #
//#Fonction: Cette bibliothèque permet de faire marcher les capteurs             #
//#                                                                              #
//################################################################################
#include "capteur.h"
#include <Math.h>
#include <Arduino.h>
#include "OpticalEncodersLib.h"
#include "espcom.h"
#include "positionLib.h"

int pos = 0;
float angle_cap;
int offset = 0;
int vitesse = 0;
float temps = 1;


//création des objets capteur ultrason
Ultrasonic ultrasonicAvant(9, 10); // Trig et Echo
Ultrasonic ultrasonicWall(11, 12); // Trig et Echo



//fonction permettant de détecter un obstacle devant le robot, jusqu'à 19cm
bool ObstacleHere(){
  int dist = ultrasonicAvant.Ranging(CM);
  return(dist <= 19);
}

//fonction qui renvoie la distance robot/mur pour permettre la cartographie
int Wall(){
  return (ultrasonicWall.Ranging(CM));
}


//Fonction permettant de caluculer la position d'un point des murs d'une salle. 
//La fonction utilise la distance renvoyée par la fonction Wall() ainsi que la position actuelle du robot
int compteur = 0;
int dist = 0;
void Scan(){
  if(compteur>10){

    //récupération de la distance
    dist = Wall();
    compteur = 0;
  }
  compteur++;
  //récupération de la position du robot
  int x = getPositionX();
  int y = getPositionY();
  //projection de la distance sur les axes x et y pour trouver les coordoonées des points
  sendPosRobot(x/5,y/5);
  x = dist * -cos(roverRotation()) + x;
  y = dist * -sin(roverRotation()) + y;

  //envoie des points pour cartographie
  sendPoint(x/5,y/5);
}

//Fonction calculant l'orientation en tout temps du robot a l'aide du gyroscope. 
//Fonction inutilisée dans la version finale
void majAngle(){
  //verification de la linéarité du mouvement du robot pour mise a jour de l'offset et éviter les dérives dans le temps
  if(isStraight())
    offset = analogRead(A0);
  else{
    //récupération de la vitesse angulaire autour de l'axe Z 
    vitesse = analogRead(A0) - offset;

    //intégration de la vitesse pour obtenir la position angulaire
    if(abs(vitesse)>=10)
      pos += vitesse * temps;
      angle_cap = pos * 0.45;
  }
}

//Fonction renvoyant l'angle calculé par le gyroscope au programme principal
//Fonction inutilisée dans la version finale
float getAngle(){
    return angle_cap;
}

