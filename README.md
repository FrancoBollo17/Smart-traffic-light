# 🚦 Ambulance Siren Detection for Smart Traffic Light

[![Python](https://img.shields.io/badge/Python-3.8%2B-blue.svg)](https://www.python.org/)
[![Arduino](https://img.shields.io/badge/Arduino-Uno%2FNano-green.svg)](https://www.arduino.cc/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-orange.svg)](https://platformio.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A real-time system that detects approaching ambulance sirens and automatically gives priority to emergency vehicles by turning all traffic lights red.

---

## 📖 Overview

This project implements an **intelligent traffic light control system** that listens to environmental sounds, detects ambulance sirens using machine learning, and commands the traffic light to switch to an emergency mode (yellow → red for all traffic ).

The system is designed to be:
- **Real-time** – processes audio in 2-second windows
- **Accurate** – uses an SVM classifier trained on MFCC features
- **Robust** – handles urban noise (traffic, horns, motorcycles)
- **Modular** – separates audio acquisition (Arduino) from processing (Python)

---

## 🏗️ System Architecture
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│ Arduino │ │ PC / Python │ │ Traffic Light │
│ (Microphone) │────▶│ (Classifier) │────▶│ (Arduino) │
│ │ │ │ │ │
│ - Reads audio │ │ - Receives data │ │ - LED control │
│ - Sends via USB │ │ - MFCC extract │ │ - Emergency │
│ - 8 kHz sample │ │ - SVM inference │ │ - Normal cycle │
└─────────────────┘ └─────────────────┘ └─────────────────┘
│ │ │
└───────────────────────┴────────────────────────┘
Serial Communication (USB)

text

**How it works:**

1. **Arduino** reads the microphone ( MAX9814) at ~8 kHz and streams raw bytes via USB Serial.
2. **Python** receives the data, buffers 2-second chunks, extracts MFCC features, and classifies the sound using a pre-trained SVM model.
3. If a siren is detected with high probability, Python sends an emergency command (`'E'`) to Arduino.
4. **Arduino** executes the emergency sequence: yellow (2s) → all red (10s) → if the emergency persists arduino enters another 3 second cycle until the siren stops.
5. If the siren fades before the 10 seconds of the initial cycle , Python sends a resume command (`'F'`).

---

## 🧠 Machine Learning Approach

- **Features**: 20 Mel-Frequency Cepstral Coefficients (MFCCs) averaged over time
- **Classifier**: Support Vector Machine (SVM) with linear kernel
- **Training data**: 
  - **Siren**: 300+ real ambulance recordings (Urban 8k and IDMT-traffic)
  - **Non-siren**: UrbanSound8K and IDMT-traffic dataset (traffic, horns, motorcycles, speech, etc.)
- **Accuracy**: ~98% on test set (balanced with `class_weight='balanced'`)

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
|-----------|----------|-------|
| Arduino Uno  | 1 |
| Microphone Module | 1 | MAX9814 ( |
| LEDs (or traffic light module) | 4+ | Green, Yellow, Red for both directions |
| USB Cable | 1 | Data + power |
| Breadboard & Jumpers | - | For prototyping |

> ℹ️ **Microphone choice**: For production, use MAX9814 (has AGC and filtering).

---

## 📁 Project Structure
.
├── arduino/
│ └── Semaforo.ino # Controls the Microphone and tells the Traffic light module what to do
├── python/
│ ├── train.py # SVM training script
│ ├── siren_model.pkl #
│ ├── scaler.pkl
│ └── label_encoder.pkl
├── data/
│ ├── siren/ # .wav files of sirens (training)
│ └── non_siren/ # .wav files of noise (training)
├── platformio.ini # PlatformIO configuration
└── README.md # This file

---

## 🔧 Installation & Setup

### 1. Clone the repository

git clone https://github.com/yourusername/ambulance-siren-detector.git
cd ambulance-siren-detector
2. Hardware wiring
Arduino Pin	Component
A0	Microphone OUT
2	Green LED (horizontal)
3	Yellow LED (horizontal)
4	Red LED (horizontal)
6	Green LED (vertical)
7	Yellow LED (vertical)
8	Red LED (vertical)
5V / GND	Power & Ground
3. Install PlatformIO (VS Code extension)
Install VS Code

Install the PlatformIO IDE extension

Open the project folder in VS Code

4. Upload Arduino code
bash
# In VS Code terminal:
pio run --target upload
5. Set up Python environment
bash
python -m venv venv
source venv/bin/activate      # Linux/Mac
# or
venv\Scripts\activate         # Windows

pip install -r requirements.txt
6. Train the model (optional – use your own audio)
Place your training .wav files in data/siren/ and data/non_siren/, then run:

bash
python python/train.py
This will generate siren_model.pkl, scaler.pkl, and label_encoder.pkl.

🚀 Usage
bash
python python/detect_arduino.py
Connects to Arduino on COM4 (change in script)

Listens to audio stream from Arduino

Prints predictions and activates traffic light on siren detection

📊 Training your own model
To improve accuracy for your local sirens:

Collect audio: 30+ siren clips + 50+ non-siren clips (2 sec each)

Place files in data/siren/ and data/non_siren/

Run training:

bash
python python/train.py
Test the model:

bash
python python/detect.py
The model uses class_weight='balanced' to handle imbalanced datasets.

📈 Performance
Metric	Value
Accuracy (test set)	~98%
Inference latency	< 0.5s (on PC)
Detection window	2 seconds
Arduino sample rate	8 kHz (adjustable)
Serial baud rate	115200
False positive rate	< 5% (with proper training)
🧠 Future Improvements
Implement Doppler shift analysis to estimate distance and direction

Add multi‑microphone array for direction‑of‑arrival (TDOA)

Deploy model on Raspberry Pi for standalone operation

Real‑time visualization of audio spectrum and detection confidence

Support for multiple emergency vehicles  (fire trucks, police) and in different countries with different sirens


📄 License
This project is licensed under the MIT License 

🙏 Acknowledgements
UrbanSound8K for non-siren audio samples

librosa for audio feature extraction

scikit-learn for SVM implementation

PlatformIO and Arduino communities for hardware support

Few-Shot Emergency Siren Detection by Michela Cantarini, Leonardo Gabrielli and Stefano Squartini for the idea

