//################################################################################
//#Auteur: BLAYES Hugo                                                           #
//#Fonction: Cette bibliothèque permet la communication entre l'ESP et l'Arduino #
//#                                                                              #
//################################################################################
#ifndef espcom_H
#define espcom_H

void setup_ESPCOM();
void sendPoint(int x, int y);
void sendPosRobot(int x, int y);
void sendFT();
unsigned char getCharacterESP();

#endif