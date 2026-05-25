# Sistem de Securitate Smart cu ESP32-CAM

Proiect realizat la materia Proiectarea cu Microprocesoare, 2026.

## Descriere

Sistem de alarma inteligent bazat pe Arduino Nano si ESP32-CAM care detecteaza intrusi prin senzori IR si microfon, emite alarma sonora, afiseaza statusul pe LCD si trimite notificari pe Telegram cu link la camera live.

## Componente

- Arduino Nano (ATmega328P)
- ESP32-CAM cu camera OV2640
- LCD 1602 cu modul I2C
- Senzor IR de proximitate MH-B
- Senzor microfon MH
- Buzzer pasiv
- FT232RL (programare ESP32-CAM)
- USB Hub TP-Link

## Functionalitati

- Detectie intrusi prin senzor IR
- Detectie zgomot prin microfon cu intreruperi hardware
- Alarma sonora diferentiata (ascendent pentru IR, descendent pentru audio)
- Afisare status pe LCD 1602 prin I2C
- Streaming video live prin WiFi
- Notificare automata pe Telegram cu link la camera

## Notiuni din laboratoare

- GPIO
- I2C
- Intreruperi hardware
- PWM
- UART

## Structura repository

- `arduino_nano/` - codul pentru Arduino Nano
- `esp32_cam/` - codul pentru ESP32-CAM
