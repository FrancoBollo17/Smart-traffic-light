# 🚦 Ambulance Siren Detection for Smart Traffic Light

[![Python](https://img.shields.io/badge/Python-3.8%2B-blue.svg)](https://www.python.org/)
[![Arduino](https://img.shields.io/badge/Arduino-Uno%2FNano-green.svg)](https://www.arduino.cc/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-orange.svg)](https://platformio.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A real-time system that detects approaching ambulance sirens and automatically gives priority to emergency vehicles by turning traffic lights green.

---

## 📖 Overview

This project implements an **intelligent traffic light control system** that listens to environmental sounds, detects ambulance sirens using machine learning, and commands the traffic light to switch to an emergency mode (yellow → red for cross traffic → green for the ambulance route).

The system is designed to be:
- **Real-time** – processes audio in 2-second windows
- **Accurate** – uses an SVM classifier trained on MFCC features
- **Robust** – handles urban noise (traffic, horns, motorcycles)
- **Modular** – separates audio acquisition (Arduino) from processing (Python)

---

## 🏗️ System Architecture
