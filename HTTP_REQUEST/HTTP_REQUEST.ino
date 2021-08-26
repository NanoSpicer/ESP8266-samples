/**
   Serial STD = Terminal

            RX  TX
   Serial1  19  18
 */
 
#define bauds 115200

void setup() {
  // Inicialitzam les connexions serial.
  Serial.begin(bauds);
  Serial1.begin(bauds);
  // Esperam a que aquestes estiguin llestes.
  while(!Serial || !Serial1);   
  Serial.println("READY");
}

void loop() {

  Serial1.write("AT+CIPSTART=\"TCP\",\"192.168.1.3\",80\r\n");
  delay(1000);
  byte b;
  // Mostram la resposta del modul quan s'ha obert la conexio
  // al port i adreça especificats.
  while (Serial1.available() > 0) {
    b = Serial1.read();
    Serial.write(b);
  }

  // Peticio GET de la arrel del webserver.
  String request = "GET / HTTP/1.0\r\n\r\n";
  // Comanda a executar.
  String cipsend = "AT+CIPSEND=";

  // Completam la comanda afegint-hi
  //    1. Els bytes a enviar
  //    2. La cua de les comanes AT
  cipsend += request.length();
  cipsend += "\r\n";
  // Executam la comanda
  Serial1.print(cipsend);
  // Esperam a que es processi.
  delay(1000);
  // Mostram la resposta del mòdul
  while (Serial1.available() > 0) {
    b = Serial1.read();
    Serial.write(b);
  }

  // Realitzam la peticio HTTP
  Serial1.print(request);
  // Bloquejam la placa mostrant tota la INFO
  // que ens envii el webserver.
  while (true) {
    if (Serial1.available() > 0) {
      b = Serial1.read();
      Serial.write(b);
    }
  }
  
}
