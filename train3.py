import os
import numpy as np
import librosa
import joblib
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn.metrics import classification_report, accuracy_score
from sklearn.preprocessing import LabelEncoder

# ==================================================
# CONFIGURAZIONE
# ==================================================
DATA_DIR = "data"          # cartella con sottocartelle "siren" e "non_siren"
DURATA_SEGMENTO = 2.0      # secondi (usa la stessa durata nel detect!)
SR = 16000                 # frequenza di campionamento (usa la stessa nel detect)
N_MFCC = 20                # numero di coefficienti MFCC

def extract_features(file_path, duration=DURATA_SEGMENTO, sr=SR):
    """Estrae MFCC medi da un file audio, tagliando/riempiendo a durata fissa."""
    try:
        # Carica il file, forza la durata e il sample rate
        audio, _ = librosa.load(file_path, sr=sr, duration=duration, res_type='kaiser_fast')
        # Se l'audio è più corto, lo zero-pad; se più lungo, viene già tagliato da duration
        if len(audio) < sr * duration:
            audio = np.pad(audio, (0, int(sr * duration) - len(audio)))
        # Calcola MFCC
        mfcc = librosa.feature.mfcc(y=audio, sr=sr, n_mfcc=N_MFCC)
        # Media temporale (un vettore per file)
        return np.mean(mfcc.T, axis=0)
    except Exception as e:
        print(f"Errore su {file_path}: {e}")
        return None

def load_data():
    features = []
    labels = []
    for label_name in ['sirena', 'non_sirena']:
        folder = os.path.join(DATA_DIR, label_name)
        if not os.path.exists(folder):
            print(f"Cartella non trovata: {folder}")
            continue
        for fname in os.listdir(folder):
            if fname.lower().endswith('.wav'):
                path = os.path.join(folder, fname)
                feat = extract_features(path)
                if feat is not None:
                    features.append(feat)
                    labels.append(label_name)
                    print(f"Caricato {label_name}: {fname}")
    return np.array(features), np.array(labels)

def main():
    print("Caricamento dati...")
    X, y = load_data()
    if len(X) == 0:
        print("Nessun file .wav trovato.")
        return
    print(f"Totale campioni: {len(X)}")

    # Codifica etichette
    le = LabelEncoder()
    y_enc = le.fit_transform(y)  # 1 = siren, 0 = non_siren

    # Split
    X_train, X_test, y_train, y_test = train_test_split(X, y_enc, test_size=0.2, random_state=42, stratify=y_enc)

    # Normalizzazione
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)

    # Modello SVM con kernel lineare (leggero e efficace)
    model = SVC(kernel='linear', C=1.0, probability=True, random_state=42)
    model.fit(X_train_scaled, y_train)

    # Valutazione
    y_pred = model.predict(X_test_scaled)
    acc = accuracy_score(y_test, y_pred)
    print(f"\nAccuratezza su test: {acc*100:.2f}%")
    print(classification_report(y_test, y_pred, target_names=['non_siren', 'siren']))

    # Salva modello, scaler e label encoder
    joblib.dump(model, 'siren_model.pkl')
    joblib.dump(scaler, 'scaler.pkl')
    joblib.dump(le, 'label_encoder.pkl')
    print("\nModello e normalizzatore salvati come 'siren_model.pkl', 'scaler.pkl', 'label_encoder.pkl'.")

if __name__ == "__main__":
    main()