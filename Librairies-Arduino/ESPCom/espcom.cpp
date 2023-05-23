#include "espcom.h"
#include "Arduino.h"

void setup_ESPCOM(){
  Serial2.begin(115200);
}

void sendPoint(int x, int y){
 String xs = String(x);
 String ys = String(y);

 Serial2.print("PX"+xs+"Y"+ys+"F");
}

void sendPosRobot(int x, int y){
 String xs = String(x);
 String ys = String(y);

 Serial2.print("RX"+xs+"Y"+ys+"F");
}

void sendFT(){
  Serial2.print("$FT");
}

unsigned char getCharacterESP(){
  if(Serial2.available()){
    return Serial2.read();
  }
  return 'N';
}
