//################################################################################
//#Auteur: BLAYES Hugo                                                           #
//#Fonction: Cette bibliothèque permet la communication entre l'ESP et l'Arduino #
//#                                                                              #
//################################################################################
#include "espcom.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

//Création de l'objet permettant la communication Serial avec l'ESP
SoftwareSerial SerialS2(8,13);
//13 Tx

//Cette fonction permet de lancer la communication avec l'ESP
void setup_ESPCOM(){
  pinMode(8,INPUT);
  pinMode(13,OUTPUT);
  SerialS2.begin(115200);
}

//Cette fonction permet de communiquer lorsqu'un point est trouvé par le robot
void sendPoint(int x, int y){
 String xs = String(x);
 String ys = String(y);

 SerialS2.print("PX"+xs+"Y"+ys+"F");
}

//Cette fonction permet de communiquer la position du robot avec l'ESP
void sendPosRobot(int x, int y){
 String xs = String(x);
 String ys = String(y);

 SerialS2.print("RX"+xs+"Y"+ys+"F");
}

//Cette fonction permet de communiquer la fin du tracé Automatique avec l'ESP
void sendFT(){
  SerialS2.print("$FT");
}

//Cette fonction permet de récupérer les commandes envoyé via l'ESP
unsigned char getCharacterESP(){
  if(SerialS2.available()){
    return SerialS2.read();
  }
  return 'N';
}
