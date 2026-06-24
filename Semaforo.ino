#include <Arduino.h>
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
int attesa ( unsigned int);
bool Controllo_seriale(char);
void Emergenza();
void Semaforo_Rosso_O();
void Semaforo_Giallo_O();
void Semaforo_Verde_O();
void Semaforo_Rosso_V();
void Semaforo_Giallo_V();
void Semaforo_Verde_V();
void Semaforo_Giallo_G();
void Semaforo_Rosso_G(); 
void Semaforo_Giallo_L();


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
    int rawVal = analogRead(PIN_MIC);
    byte binSample = (byte)(rawVal >> 2);
    Serial.write(binSample);
 }
      char Terminale = Serial.read();
      int caso=0;
      if (Terminale == ('I')){
 while(Terminale != ('N')) {
      Semaforo_Rosso_O;
      caso=attesa(rosso);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
      Semaforo_Giallo_V;
      caso=attesa(giallo);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
      Semaforo_Verde_O;
      caso=attesa(verde);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
      Semaforo_Giallo_O;
      caso=attesa(giallo);
      if(caso == 1){
      Emergenza();
      }
      else if (caso == 3){
        break;
      }
    
 }
   while (true){
    Semaforo_Giallo_L;
   }
   
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
  void Semaforo_Giallo_L() //lampeggiante
  {
    digitalWrite(PIN_VERDE_O,LOW);
    digitalWrite(PIN_GIALLO_O,HIGH);
    digitalWrite(PIN_ROSSO_O,LOW);
    // verticale acceso
    digitalWrite(PIN_VERDE_V,LOW);
    digitalWrite(PIN_GIALLO_V,HIGH);
    digitalWrite(PIN_ROSSO_V,LOW);
    delay(500);
    //orizzontale spento
    digitalWrite(PIN_GIALLO_O,LOW);
    // verticale spento
    digitalWrite(PIN_GIALLO_V,LOW);
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
  return 4;
}

int attesa_emergenza(unsigned int delay){
  unsigned long int Tempo_inizio=millis();
  while(millis()-Tempo_inizio<delay){
     if (micros() - lastSampleTime >= SAMPLING_INTERVAL) {
      //invio campioni a python
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
void Emergenza(){;
      Semaforo_Giallo_G;
      delay(3000);
      Semaforo_Rosso_G; 
      int caso=attesa_emergenza(10000);
      while(caso == 3){
        Semaforo_Rosso_G;
        caso=attesa_emergenza(3000);
      }
}
