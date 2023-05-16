#include "espcom.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial SerialS2(8,13);
//13 Tx

void setup_ESPCOM(){
  pinMode(8,INPUT);
  pinMode(13,OUTPUT);
  SerialS2.begin(115200);
}

void sendPoint(int x, int y){
 String xs = String(x);
 String ys = String(y);

 SerialS2.print("PX"+xs+"Y"+ys+"F");
}

void sendPosRobot(int x, int y){
 String xs = String(x);
 String ys = String(y);

 SerialS2.print("RX"+xs+"Y"+ys+"F");
}

void sendFT(){
  SerialS2.print("$FT");
}

unsigned char getCharacterESP(){
  if(SerialS2.available()){
    return SerialS2.read();
  }
  return 'N';
}
