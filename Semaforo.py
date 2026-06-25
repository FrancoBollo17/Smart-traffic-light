import serial
import numpy as np
import joblib
import librosa
import time

# ========== CONFIGURAZIONE ==========
PORTA_SERIALE = 'COM4'
BAUD = 115200
DURATA_BLOCCO = 2.0
SR_TARGET = 16000
SR_ARDUINO = 8000

SOGLIA_PROB = 0.65
TIMEOUT_SIRENA = 10.0  

# ========== CARICA MODELLO ==========
print("Caricamento modello...")
model = joblib.load('siren_model.pkl')
scaler = joblib.load('scaler.pkl')
label_encoder = joblib.load('label_encoder.pkl')
print("Modello caricato.")

# ========== COLLEGA AD ARDUINO ==========
try:
    ser = serial.Serial(PORTA_SERIALE, BAUD, timeout=0) 
    print(f"Collegato a {PORTA_SERIALE}")
except Exception as e:
    print(f"Errore connessione, collega arduino o chiudi i programmi che usano la seriale : {e}")
    exit()

def invia_comando(cmd):
    ser.write(cmd.encode())
    print(f"Comando: {cmd}")

def extract_features(audio_chunk, sr_originale):
    if sr_originale != SR_TARGET:
        audio_chunk = librosa.resample(audio_chunk, orig_sr=sr_originale, target_sr=SR_TARGET)
    if len(audio_chunk) < int(SR_TARGET * DURATA_BLOCCO):
        audio_chunk = np.pad(audio_chunk, (0, int(SR_TARGET * DURATA_BLOCCO) - len(audio_chunk))) #aggiunge degli 0 se i campioni non sono 32000
    else:
        audio_chunk = audio_chunk[:int(SR_TARGET * DURATA_BLOCCO)]
    mfcc = librosa.feature.mfcc(y=audio_chunk, sr=SR_TARGET, n_mfcc=20) #fa i coefficienti mfcc del audio
    return np.mean(mfcc.T, axis=0) #calcola la media e trasposta la matrice

#Analizza il suono e lo confronta con i dati presenti nei file pkl
def Analisi_Suono():
    global buffer 
    
  
    if ser.in_waiting > 0:
        data = ser.read(ser.in_waiting)
        for byte in data:
            buffer.append(byte)

    if len(buffer) >= campioni_per_blocco:
        chunk_raw = np.array(buffer[:campioni_per_blocco], dtype=np.float32) #converte in array di float32 per l'analisi con MFCC
        buffer = buffer[campioni_per_blocco:] # Svuota i campioni usati
 
        chunk = (chunk_raw - 128) / 128.0
        feat = extract_features(chunk, SR_ARDUINO)
        feat_scaled = scaler.transform([feat]) #normalizza
        
       
        prob = model.predict_proba(feat_scaled)[0]
        pred = model.predict(feat_scaled)[0]
        classe = label_encoder.inverse_transform([pred])[0] #etichetta il tipo di dato
        prob_siren = prob[1] if len(prob) > 1 else (1 if pred == 1 else 0)
        
        print(f"Pred: {classe} (prob sirena={prob_siren:.2f})   ", end='\r')
        return classe, prob_siren
        
    
    return None, 0.0


#attiva semaforo
def Attivazione_semaforo(classe, prob_siren):
    global emergenza_attiva, ultimo_rilevamento, buffer
    ora = time.time()
    
    
    if classe == 'sirena' and prob_siren > SOGLIA_PROB:
        ultimo_rilevamento = ora 
        
        if not emergenza_attiva:
            emergenza_attiva = True
            print(f"\n AMBULANZA RILEVATA! Attivo Emergenza.")
            invia_comando("E") 
            classe, prob_siren = Analisi_Suono()
            while(classe == 'sirena' and prob_siren>SOGLIA_PROB):
                classe, prob_siren = Analisi_Suono()
                invia_comando(f"P{prob_siren:.2f}")
                time.sleep(3)
                print("\n Prolungo il rosso")
            

    if emergenza_attiva and (ora - ultimo_rilevamento > TIMEOUT_SIRENA):
        emergenza_attiva = False
        print("\n Sirena non più rilevata. Ripristino il normale ciclo.")
        invia_comando("F") 
        ser.reset_input_buffer()
        buffer.clear()


#interfaccia
buffer = []
emergenza_attiva = False
ultimo_rilevamento = 0.0
campioni_per_blocco = int(SR_ARDUINO * DURATA_BLOCCO)


print("\nRilevatore in ascolto... (Ctrl+C per fermare)\n")

time.sleep(2)
invia_comando("I") 
print(f"Semaforo azionato")


#ciclo
try:
    while True:
        risultato_classe, risultato_prob = Analisi_Suono()
        
        Attivazione_semaforo(risultato_classe, risultato_prob)
        
        time.sleep(0.001)
#spegne il codice
except KeyboardInterrupt:
    print("\nChiusura...")
    invia_comando("N") #attiva il semaforo giallo lampeggiante
    ser.close()
