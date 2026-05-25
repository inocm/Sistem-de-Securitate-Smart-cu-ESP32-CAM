#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IR1_PIN 2
#define MIC_PIN 3
#define BUZZER_PIN 8
#define TRIGGER_PIN 9

volatile bool micDetectat = false;

void micISR() {
  micDetectat = true;
}

void setup() {
  Serial.begin(9600);
  pinMode(IR1_PIN, INPUT);
  pinMode(MIC_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(MIC_PIN), micISR, RISING);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistem gata!");
  lcd.setCursor(0, 1);
  lcd.print("Monitorizare...");
}

void loop() {
  int ir1 = digitalRead(IR1_PIN);

  Serial.print("IR1: "); Serial.print(ir1);
  Serial.print(" MIC: "); Serial.println(micDetectat);

  bool irDetectat = (ir1 == LOW);
  bool audDetectat = micDetectat;
  bool intrus = (irDetectat || audDetectat);

  if (intrus) {
    digitalWrite(TRIGGER_PIN, HIGH);
    Serial.println("Trimit trigger la ESP32!");
    lcd.setCursor(0, 0);
    lcd.print("!! INTRUS !!    ");
    lcd.setCursor(0, 1);

    if (irDetectat && audDetectat) {
      lcd.print("Tip: IR + AUD   ");
    } else if (irDetectat) {
      lcd.print("Tip: IR         ");
    } else if (audDetectat) {
      lcd.print("Tip: AUD        ");
    }

    if (audDetectat && !irDetectat) {
      // Sunet descendent AUD
      for (int freq = 2000; freq >= 500; freq -= 50) {
        tone(BUZZER_PIN, freq);
        delay(20);
      }
      noTone(BUZZER_PIN);
      delay(500);
    } else {
      // Sunet ascendent IR
      for (int freq = 500; freq <= 2000; freq += 50) {
        tone(BUZZER_PIN, freq);
        delay(20);
      }
      noTone(BUZZER_PIN);
      delay(500);
    }

    micDetectat = false;

  } else {
    digitalWrite(TRIGGER_PIN, LOW);
    noTone(BUZZER_PIN);
    lcd.setCursor(0, 0);
    lcd.print("Sistem gata!    ");
    lcd.setCursor(0, 1);
    lcd.print("Monitorizare... ");
  }
}