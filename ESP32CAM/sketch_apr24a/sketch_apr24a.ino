//##############################################
//#Auteur: BLAYES Hugo                         #
//#Fonction: Programme ESP pour le PFR4        #
//#                                            #
//##############################################
#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <WebServer.h>  

//Variable Globale
String positionRobot;

String point = "";
String FT = "";
String HTML;
String HTML_top = "<!DOCTYPE html>\
                    <html>\
                       <body>\
                       <h1>";
String HTML_bottom = "</h1>\
                      </body>\
                      </html>";

//Déclaration du SSID et du PASSWORD de notre WebServer
const char* ssid = "PFR3_WiFi_3";
const char* password = "123456789";

//Déclaration de notre WebServer sur le port 80
WebServer server(80);

void setup() {
  // Initialisation du port Serie avec lequel l'ESP communiquera avec l'Arduino
  Serial.begin(115200);
  while(!Serial);

  //Configuration du ServerWeb
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  //Création de la page HTML qui va être envoyé lorque l'on requête nos pages Web
  HTML = HTML_top + point + "<br>" + FT + HTML_bottom;
  //Initialisation de la caméra
  InitialiserCamera();

  //Déclaration des différentes pages 
  server.on("/",handle_root);
  server.on("/G",handle_G);
  server.on("/D",handle_D);
  server.on("/B",handle_B);
  server.on("/H",handle_H);
  server.on("/A",handle_A);
  server.on("/M",handle_M);
  server.on("/robot",handle_robot);
  server.on("/image",HTTP_GET, FluxImage);
  server.on("/S",handle_S);

  //Lancement du Server
  server.begin();
}

void loop() {
  // Reecriture de la page HTML
  HTML = HTML_top + point + FT + HTML_bottom;
  server.handleClient();
  byte r;
  //Tant que le Rover est alimenté on lit le port Serie afin de savoir si des données sont envoyé sur les ports de l'ESP
  if(Serial.available() > 0){
    r = Serial.read();
    if(r=='P')commuPoint();
    if(r=='R')commuRobot();
    if(r=='$')commuFinTrame();
  }
}

//Si la page principale est appellée on envoie la liste des points reçu par notre ESP
void handle_root(){
  server.send(200,"text/html",HTML);
  point = "";
  FT = "";
}

//Si cette page est appelé on envoie des 'S' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_S(){
  for(int i=0;i<5;i++)Serial.print("S");
  server.send(200,"text/html",HTML_top+HTML_bottom);
}

//Si cette page est appelé on envoie des 'G' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_G(){
  for(int i=0;i<5;i++)Serial.print("G");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

//Si cette page est appelé on envoie des 'D' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_D(){
  for(int i=0;i<5;i++)Serial.print("D");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

//Si cette page est appelé on envoie des 'B' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_B(){
  for(int i=0;i<5;i++)Serial.print("B");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

//Si cette page est appelé on envoie des 'H' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_H(){
  for(int i=0;i<5;i++)Serial.print("H");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

//Si cette page est appelé on envoie des 'A' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_A(){
  for(int i=0;i<5;i++)Serial.print("A");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

//Si cette page est appelé on envoie des 'M' sur le port Série et en renvoie une page vide à l'utilisateur
void handle_M(){
  for(int i=0;i<5;i++)Serial.print("M");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

//Si cette page est appelé on envoie la position du robot à l'utilisateur
void handle_robot(){
  server.send(200,"text/html",HTML_top+positionRobot+HTML_bottom);
}

//Fonction permettant de lire et de mettre en forme les points
void commuPoint(){
  char c;
  String point_tempo;
  while(c!='F'){
    if(Serial.available()>0){
      c = Serial.read();
      if(!(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9' || c=='X' || c=='Y' || c=='F'))return;
      point_tempo += c; 
    }
  }
 point += point_tempo + "<br>";
}

//Fonction permettant de lire et de mettre en forme les points et de les rajouter à la prochaine page Web
void commuRobot(){
  char c;
  String point_tempo;
  positionRobot = "";
  while(c!='F'){
    if(Serial.available()>0){
      c = Serial.read();
      if(!(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9' || c=='X' || c=='Y' || c=='F'))return;
      positionRobot += c; 
    }
  }
 positionRobot += "<br>";
}

//Fonction permettant d'envoyer la fin de tracé
void commuFinTrame(){
  char c;
  String trame_Tempo;
  while(c!='T'){
    if(Serial.available()>0){
      c = Serial.read();
      trame_Tempo += c;
    }
  }
  FT = trame_Tempo;
}

//Fonction permettant d'initialiser les tampons de la caméra
esp_err_t InitialiserCamera(){
    // Variables locales de la fonction
    esp_err_t       Retour_L;
    camera_config_t ConfigurationCamera_L;
    // Cablage de la caméra sur l'ESP32-CAM du fabricant AI-Thinker
    ConfigurationCamera_L.pin_d0 = 5;
    ConfigurationCamera_L.pin_d1 = 18;
    ConfigurationCamera_L.pin_d2 = 19;
    ConfigurationCamera_L.pin_d3 = 21;
    ConfigurationCamera_L.pin_d4 = 36;
    ConfigurationCamera_L.pin_d5 = 39;
    ConfigurationCamera_L.pin_d6 = 34;
    ConfigurationCamera_L.pin_d7 = 35;
    ConfigurationCamera_L.pin_xclk = 0;
    ConfigurationCamera_L.pin_pclk = 22;
    ConfigurationCamera_L.pin_vsync = 25;
    ConfigurationCamera_L.pin_href = 23;
    ConfigurationCamera_L.pin_sscb_sda = 26;
    ConfigurationCamera_L.pin_sscb_scl = 27;
    ConfigurationCamera_L.pin_pwdn = 32;
    ConfigurationCamera_L.pin_reset = -1;
    // La génération du signal d'horloge
    ConfigurationCamera_L.ledc_channel = LEDC_CHANNEL_0;
    ConfigurationCamera_L.ledc_timer = LEDC_TIMER_0;
    ConfigurationCamera_L.xclk_freq_hz = 20000000;
    // Compression jpeg
    ConfigurationCamera_L.pixel_format = PIXFORMAT_JPEG;
    ConfigurationCamera_L.fb_count = 2;
    ConfigurationCamera_L.jpeg_quality = 10;
    // Résolution de l'image
    ConfigurationCamera_L.frame_size = FRAMESIZE_SVGA;
    // Lancement de l'initialisation de la caméra
    Retour_L=esp_camera_init(&ConfigurationCamera_L);
    if (Retour_L == ESP_OK)
    {
        Serial.printf("La camera est initialisee\n");
    }
    else
    {
        Serial.printf("Erreur 0x%x lors de l'initialisation de la camera\n", Retour_L);
    }
    return(Retour_L);
}

//Fontion permettant de charger l'image prise par la caméra à l'appel de la fonction et de l'envoyer à l'utilisateur 
void FluxImage(){
    camera_fb_t* fb = esp_camera_fb_get();
    server.setContentLength(fb->len);
    server.sendHeader("Content-Type", "image/jpeg");
    server.send(200);

    WiFiClient client = server.client();
    client.write(fb->buf, fb->len);
    // Réamorcage du tampon video, indispensable pour sa réutilisation
    esp_camera_fb_return(fb);  
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@&@@@@@@&@@@@%%%@%&@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@&@@@@@%%%%&%%&&@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@%@@@@@@@@%%%@@@@@@@@&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@&@@@@@@@%%@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@%@@@@@@@@@%@%%@@@@@@@@@@@%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@&@@@@@@@@@%%%%@@@@@@@@@&%%%%@@@%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@&@@@@@@@@%%&%%%@@@@@@@@%%%%%&@@@@%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@&@@@@@@@@@@%@&%%%@@@@@@%%%%%%@@@@@@%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@%@@@@@@@@@@@@@%%%@@@@%%%%%%@@@@@@@@@@&%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@%@@@@@@@@@@@@@@%%@@%%%%%@@@@@@@@@@@@@@@&%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@@%@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@&%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@%@@@@@@@@@@%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@%%%@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@&%%%%@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&@@@&%%
*/