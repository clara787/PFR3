#include "positionLib.h"
#include "OpticalEncodersLib.h"
#include <Math.h>
#include "capteur.h"

float x = 0;
float y = 0;

float getPositionX(){
    return x;
}

float getPositionY(){
    return y;
}

void newPosition(){
    //recup distance fonction marc
    long rightDist = getRightCounter();
    long leftDist = getLeftCounter();
    float distRight = counterToCm(rightDist);
    float distLeft = counterToCm(leftDist);
    float dist = rightDist>leftDist?rightDist:leftDist;
    //angle = calcul angle avec getAngle();
    x += dist * cos(getAngle()); //getAngle de Rahim
    y += dist * sin(getAngle());
}