#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <WebServer.h>  

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
const char* ssid = "PFR3_WiFi_3";
const char* password = "123456789";

WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  HTML = HTML_top + point + "<br>" + FT + HTML_bottom;
  InitialiserCamera();

  server.on("/",handle_root);
  server.on("/G",handle_G);
  server.on("/D",handle_D);
  server.on("/B",handle_B);
  server.on("/H",handle_H);
  server.on("/A",handle_A);
  server.on("/M",handle_M);
  server.on("/robot",handle_robot);
  server.on("/stream",HTTP_GET, FluxVideo);
  server.on("/image",HTTP_GET, FluxImage);
  server.on("/S",handle_S);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  HTML = HTML_top + point + FT + HTML_bottom;
  server.handleClient();
  byte r;
  if(Serial.available() > 0){
    r = Serial.read();
    if(r=='P')commuPoint();
    if(r=='R')commuRobot();
    if(r=='$')commuFinTrame();
  }
}

void handle_root(){
  server.send(200,"text/html",HTML);
  point = "";
  FT = "";
}

void handle_S(){
  for(int i=0;i<5;i++)Serial.print("S");
  server.send(200,"text/html",HTML_top+HTML_bottom);
}

void handle_G(){
  for(int i=0;i<5;i++)Serial.print("G");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

void handle_D(){
  for(int i=0;i<5;i++)Serial.print("D");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

void handle_B(){
  for(int i=0;i<5;i++)Serial.print("B");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

void handle_H(){
  for(int i=0;i<5;i++)Serial.print("H");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

void handle_A(){
  for(int i=0;i<5;i++)Serial.print("A");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

void handle_M(){
  for(int i=0;i<5;i++)Serial.print("M");
  server.send(200,"text/html",HTML_top + HTML_bottom);
}

void handle_robot(){
  server.send(200,"text/html",HTML_top+positionRobot+HTML_bottom);
}

void commuPoint(){
  char c;
  String point_tempo;
  while(c!='F'){
    if(Serial.available()>0){
      c = Serial.read();
      point_tempo += c; 
    }
  }
 point += point_tempo + "<br>";
}

void commuRobot(){
  char c;
  String point_tempo;
  positionRobot = "";
  while(c!='F'){
    if(Serial.available()>0){
      c = Serial.read();
      positionRobot += c; 
    }
  }
 positionRobot += "<br>";
}

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

void FluxVideo(){
    String EnteteHtmlInitiale_L = "HTTP/1.1 200 OK\r\n" \
        "Access-Control-Allow-Origin: *\r\n" \
        "Content-Type: multipart/x-mixed-replace; boundary=FinDeLaSection\r\n\r\n;";
    String EnteteHtmlIntermediaire_L = "--FinDeLaSection\r\n" \
        "Content-Type: image/jpeg\r\n\r\n";
    WiFiClient Client_L;
    camera_fb_t *TamponVideo_L = NULL; 
  
    server.sendContent(EnteteHtmlInitiale_L);
    Client_L = server.client();
    while (Client_L.connected())
    {   
        // Le navigateur Web client est toujours connecté
        TamponVideo_L = esp_camera_fb_get(); // Récupère une image depuis la caméra      
        server.sendContent(EnteteHtmlIntermediaire_L);
        Client_L.write((char *)TamponVideo_L->buf, TamponVideo_L->len);
        server.sendContent("\r\n");
         
        // Réamorcage du tampon video, indispensable pour sa réutilisation
        if (TamponVideo_L)
        {
            esp_camera_fb_return(TamponVideo_L);
            TamponVideo_L = NULL;
        }
    }
}

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
