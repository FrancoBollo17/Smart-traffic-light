
unsigned long lastSampleTime = 0;
const unsigned long SAMPLING_INTERVAL = 125; 
const int rosso =20000, verde=20000, giallo=3000;
#define PIN_MIC A0
#define PIN_VERDE_O 2
#define PIN_GIALLO_O 3
#define PIN_ROSSO_O 4

#define PIN_VERDE_V 6
#define PIN_GIALLO_V 7
#define PIN_ROSSO_V 8
int attesa (int);
bool Controllo_seriale(char);
void Emergenza();

void setup() {

  Serial.begin(115200); 
  pinMode(PIN_VERDE_O,OUTPUT);
  pinMode(PIN_GIALLO_O,OUTPUT);
  pinMode(PIN_ROSSO_O,OUTPUT);
  pinMode(PIN_VERDE_V,OUTPUT);
  pinMode(PIN_GIALLO_V,OUTPUT);
  pinMode(PIN_ROSSO_V,OUTPUT);
  pinMode(PIN_MIC, INPUT);
}

void loop() {
  
 if (micros() - lastSampleTime >= SAMPLING_INTERVAL) {
    lastSampleTime = micros();
    
    // Legge il valore (0-1023)
    int rawVal = analogRead(PIN_MIC);
    
    // Converte da 10-bit a 8-bit (0-255) spostando i bit di 2 posizioni a destra
    byte binSample = (byte)(rawVal >> 2);
    
    // Invia il singolo byte grezzo sulla seriale
    Serial.write(binSample);
 }
      char Terminale = Serial.read();
      int caso=0;
      if (Terminale == ('I')){
 while(Terminale != ('N')) {
      Semaforo_Rosso_O();
      caso=attesa(rosso);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
      Semaforo_Giallo_V();
      caso=attesa(giallo);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
      Semaforo_Verde_O();
      caso=attesa(verde);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
      Semaforo_Giallo_O();
      caso=attesa(giallo);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
    
 }
   Serial.write("Semaforo spento");
   return 0;
      }
      
     
   
  
}
  void Semaforo_Rosso_O(){
    //orizzontale
    digitalWrite(PIN_VERDE_O,LOW);
    digitalWrite(PIN_GIALLO_O,LOW);
    digitalWrite(PIN_ROSSO_O,HIGH);
    // verticale
    digitalWrite(PIN_VERDE_V,HIGH);
    digitalWrite(PIN_GIALLO_V,LOW);
    digitalWrite(PIN_ROSSO_V,LOW);
  }
  void Semaforo_Giallo_V(){
    //orizzontale
    digitalWrite(PIN_VERDE_O,LOW);
    digitalWrite(PIN_GIALLO_O,LOW);
    digitalWrite(PIN_ROSSO_O,HIGH);
    // verticale
    digitalWrite(PIN_VERDE_V,LOW);
    digitalWrite(PIN_GIALLO_V,HIGH);
    digitalWrite(PIN_ROSSO_V,LOW);
  }
  void Semaforo_Verde_O(){
    //orizzontale
    digitalWrite(PIN_VERDE_O,HIGH);
    digitalWrite(PIN_GIALLO_O,LOW);
    digitalWrite(PIN_ROSSO_O,LOW);
    // verticale
    digitalWrite(PIN_VERDE_V,LOW);
    digitalWrite(PIN_GIALLO_V,LOW);
    digitalWrite(PIN_ROSSO_V,HIGH);
  }
  void Semaforo_Giallo_O(){
    //orizzontale
    digitalWrite(PIN_VERDE_O,LOW);
    digitalWrite(PIN_GIALLO_O,HIGH);
    digitalWrite(PIN_ROSSO_O,LOW);
    // verticale
    digitalWrite(PIN_VERDE_V,LOW);
    digitalWrite(PIN_GIALLO_V,LOW);
    digitalWrite(PIN_ROSSO_V,HIGH);
  }
  //emergenza
  void Semaforo_Giallo_G(){
    //orizzontale
    digitalWrite(PIN_VERDE_O,LOW);
    digitalWrite(PIN_GIALLO_O,HIGH);
    digitalWrite(PIN_ROSSO_O,LOW);
    // verticale
    digitalWrite(PIN_VERDE_V,LOW);
    digitalWrite(PIN_GIALLO_V,HIGH);
    digitalWrite(PIN_ROSSO_V,LOW);
  }
  void Semaforo_Rosso_G(){
    //orizzontale
    digitalWrite(PIN_VERDE_O,LOW);
    digitalWrite(PIN_GIALLO_O,LOW);
    digitalWrite(PIN_ROSSO_O,HIGH);
    // verticale
    digitalWrite(PIN_VERDE_V,LOW);
    digitalWrite(PIN_GIALLO_V,LOW);
    digitalWrite(PIN_ROSSO_V,HIGH);
  }
int attesa(int delay){
  unsigned long int Tempo_inizio=millis();
  while(millis()-Tempo_inizio<delay){
     if (micros() - lastSampleTime >= SAMPLING_INTERVAL) {
      //invio campioni a python
    lastSampleTime = micros();
    int rawVal = analogRead(PIN_MIC);
    byte binSample = (byte)(rawVal >> 2);
    Serial.write(binSample);
    char Terminale=Serial.read();
    if (Terminale == 'E'){
      return 1;
    }  
    else if (Terminale == 'F') {
      return 2;
    }
    else if (Terminale == 'N'){
      return 3;
    }
}
  }
}

int attesa_emergenza(int delay){
  unsigned long int Tempo_inizio=millis();
  while(millis()-Tempo_inizio<delay){
     if (micros() - lastSampleTime >= SAMPLING_INTERVAL) {
    lastSampleTime = micros();
    int rawVal = analogRead(PIN_MIC);
    byte binSample = (byte)(rawVal >> 2);
    Serial.write(binSample);
    char Terminale=Serial.read();
    if (Terminale == 'F'){
      return 1;
    }  
    else if (Terminale == 'N'){
      return 2;
    }
}

  }
  return 3;
}
void Emergenza(){
      Serial.print("EMergenza");
      Semaforo_Giallo_G();
      delay(3000);
      Semaforo_Rosso_G(); 
      int caso=attesa_emergenza(10000);
      while(caso == 3){
        Serial.write("L'emergenza continua");
        Semaforo_Rosso_G();
        caso=attesa_emergenza(3000);
      }
      Serial.write("L'emergenza finita");
}