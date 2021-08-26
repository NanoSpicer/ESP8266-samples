/**
 * SKETCH QUE PERMET ENVIAR CADENES DE TEXT
 */

// Bauds al que funcionaran els serial i ESP.
#define bauds 115200

// Adreça IP del terminal amb el que ens volem connectar
const String ip_address="192.168.1.3";
// Socket/Port on ens volem connectar
const String port="2308";

// Cua que necessiten les comanes AT.
const String tail = "\r\n";

// *** Variables necessaries per simular interrupcions
// Indica si hi ha missatges pendents de mostrar.
boolean hi_ha_missatges = false;
// Missatge que està pendent de mostrar.
String input = "";





void setup() {
  // Inicialitzam les connexions Serial
  Serial.begin(bauds);
  Serial1.begin(bauds);
  // Esperam que estiguin llestes.
  while(!Serial || !Serial1);

  // Conectar amb el wifi i obrir el socket indicat
  inicialitzar();

  // Avisar de que esteim llests.
  Serial.println("READY!");
}

void loop() {
  // Si l'usuari ha escrit algun missatge el llegim
  // i l'enviam
  if(Serial.available()>0){
    String msg = Serial.readStringUntil('\n');
    // Si llegim exit, tancam conexio i bloquejam
    if(msg.equals("exit")){
      send_command("AT+CIPCLOSE");
      pinMode(13, OUTPUT);
      digitalWrite(13, LOW);
      while(true);
    }
    send_message(msg);
  }
  // Si hi ha missatges a mostrar el mostram
  if(hi_ha_missatges){
    Serial.println(skip_message(':'));
    hi_ha_missatges = false;
    input="";
  }
  // Miram si s'ha rebut alguna cosa.
  serialEvent();
}


/**
 *  Funcio que envia la comana AT especificada.
 *  @cmd = Comana AT a enviar.
 */
 void send_command(String cmd){
  // Escriure la comanda
  for(int i =0; i<cmd.length(); i++){
    Serial1.write(cmd.charAt(i));  
  }
  
  // Escriure la coa de la comanda. 
  for(int i =0; i<tail.length(); i++){
    Serial1.write(tail.charAt(i));
  }
}

/**
 * Funcio que s'encarrega de buidar l'output
 * del ESP que no ens interessa
 */
void dump_output(){
  while(Serial1.available()>0){
    Serial1.read();
  }
}

/**
 * Procediment necessari per establir connexió
 */
void inicialitzar(){

  // Desconexio amb l'AP actual.
  send_command("AT+CWQAP");
  // Esperam que es desconnecti.
  esperar("OK");

  // Ens connectam a un access point
  Serial.println("Conectant al wifi...");
  send_command("AT+CWJAP=\"FTE-7099\",\"new_password\"");
  esperar("OK");
  Serial.println("Connexio obtinguda!");

  // Quan ja ens hem connectat, miram de conectarmos a la IP
  // i ports especificats prèviament
  Serial.println("Connectant amb el socket...");
  send_command("AT+CIPSTART=\"TCP\",\""+ip_address+"\","+port);
  esperar("OK");
  
  // Informam de la connexio exitosa.
  Serial.println("Connectat a: "+ip_address+":"+port);
}

/**
 * Funcio que espera a es trobi una cadena de 
 * caracters al Serial1.
 * 
 * S'empra per esperar un cert output del modul Wi-Fi
 */
void esperar(char token[]){
  while(!Serial1.find(token));
}

/**
 * Funcio que retorna el missatge llegit del modul wifi.
 * 
 * Output del missatge:
 * +IPD,n_bytes: missatge
 * 
 * @return = missatge
 */
String skip_message(char look){
  String ret = "";
  int aux;
  
  // Cercam el caracter indicat @look
  for(int i =0; i< input.length();i++){
    if(look == input.charAt(i)){
      // Primer caracter que ens interessa
      aux = i+1;
      break;
    }
  }

  // El substring que ens interessa retornar
  while(aux<input.length()){
    ret += input.charAt(aux);
    aux++;
  }
  return ret;
}

/**
 * Funcio que encapsula totes les passes
 * necessaries per poder enviar el missatge
 * 
 * @msg = missatge a enviar.
 */
void send_message(String msg){
  send_command("AT+CIPSEND="+String(msg.length()));
  delay(500);
  for(int i =0; i<msg.length(); i++){
    Serial1.write(msg.charAt(i));
  }
  esperar("SEND OK");
  Serial.println("Missatge enviat: "+msg);
}


/**
 * Funcio que s'encarrega de llegir i notificar
 * els missatges que arriben del modul ESP.
 */
void serialEvent(){
  char aux;
  while(Serial1.available()>0){
    aux = (char) Serial1.read();
    if(aux == '\n'){
      hi_ha_missatges=true;
      while(Serial1.available())Serial1.read();
      break;
    }
    input+=aux;
  }
}

