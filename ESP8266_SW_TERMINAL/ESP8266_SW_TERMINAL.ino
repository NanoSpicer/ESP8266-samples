#include <SoftwareSerial.h>

#define bauds 115200
SoftwareSerial esp(10, 11); //RX, TX. Conectar creuats. RX ard TX modul

int second = 1000;
void setup() {
  Serial.begin(bauds);
  esp.begin(bauds);
  delay(second);
  Serial.println("SETUP READY");
}

void loop() {
   byte aux;

  while(esp.available()>0){
    aux = esp.read();
    Serial.write(aux);
  }

  while(Serial.available()>0){
    aux = Serial.read();
    esp.write(aux);
  }
}
