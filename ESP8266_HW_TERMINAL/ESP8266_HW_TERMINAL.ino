
/**
 * Serial  = Terminal
 * Serial1 = WiFi
 * 
 *                      RX   TX
 * Arduino MEGA Pins:   19   18
 * 
 * Connectar creuats els RX i TX del ESP8266-01.
 * 
 */

#define BAUDS 115200

int second = 1000;

void setup() {
  Serial.begin(BAUDS);
  Serial1.begin(BAUDS);
  delay(second);
  Serial.println("SETUP READY");
}

void loop() {
  byte aux;

  while(Serial1.available()>0){
    aux = Serial1.read();
    Serial.write(aux);
  }

  while(Serial.available()>0){
    aux = Serial.read();
    Serial1.write(aux);
  }

}
