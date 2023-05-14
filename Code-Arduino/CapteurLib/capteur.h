#include <Ultrasonic.h>

Ultrasonic ultrasonicAvant(9, 8); // Trig et Echo
Ultrasonic ultrasonicWall(11, 10); // Trig et Echo

boolean ObstacleHere();
int Wall();
void Scan();