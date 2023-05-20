#include "positionLib.h"
#include "OpticalEncodersLib.h"
#include <Math.h>
#include "capteur.h"
#include <Arduino.h>

float x = 500;
float y = 500;

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
    float dist = (distRight+distLeft)/2;
    //angle = calcul angle avec getAngle();
    x += dist * -sin(roverRotation()); //getAngle de Rahim
    y += dist * cos(roverRotation());
}

void razPos(){
  x = 500;
  y = 500;
}