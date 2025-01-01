#include <Arduino.h>

// Définir les broches pour les boutons, les LEDs et le buzzer
const int buttonPin1 = 18;
const int buttonPin2 = 19;
const int ledPin1 = 22;
const int ledPin2 = 23;
const int buzzerPin = 25;

// Variables pour l'état des boutons
bool button1Pressed = false;
bool button2Pressed = false;

// Mélodie "Victory Theme" rapide
int victoryMelody[] = { 262, 294, 330, 349, 392, 440, 494, 523, 554, 587 }; // Notes: Do, Ré, Mi, Fa, Sol, La, Si, Do, Ré, Mi
int victoryDurations[] = { 200, 200, 200, 200, 200, 200, 200, 200, 200, 200 }; // Durée rapide de chaque note (200ms)

// Mélodie "Twinkle, Twinkle, Little Star" rapide
int twinkleMelody[] = { 262, 262, 392, 392, 440, 440, 392, 349, 349, 330 }; // Notes : Do, Do, Sol, Sol, La, La, Sol, Fa, Fa, Mi
int twinkleDurations[] = { 200, 200, 200, 200, 200, 200, 400, 200, 200, 400 }; // Durée rapide de chaque note

// Variables pour la lecture des mélodies
int currentNote = 0;
unsigned long noteStartTime = 0;
bool isPlayingMelody = false;
int activeMelody = 0; // 1 pour Victory, 2 pour Twinkle

// Variables pour le délai après appui sur un bouton
unsigned long ledStartTime = 0;

// Fonction pour faire clignoter les LEDs 5 fois
void blinkLedsFiveTimes() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    delay(100); // LEDs allumées 100ms
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    delay(100); // LEDs éteintes 100ms
  }
}

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);

  // Clignotement des LEDs 5 fois à l'initialisation
  blinkLedsFiveTimes();
}

void loop() {
  // Lire les états des boutons
  bool currentButton1State = digitalRead(buttonPin1);
  bool currentButton2State = digitalRead(buttonPin2);

  // Si aucun bouton n'est appuyé et aucune mélodie n'est en cours
  if (!isPlayingMelody && !button1Pressed && !button2Pressed) {
    if (currentButton1State == LOW) {
      button1Pressed = true;
      button2Pressed = false;
      isPlayingMelody = true;
      activeMelody = 1; // Jouer "Victory Theme"
      currentNote = 0;
      noteStartTime = millis();
      ledStartTime = millis();
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
    } else if (currentButton2State == LOW) {
      button2Pressed = true;
      button1Pressed = false;
      isPlayingMelody = true;
      activeMelody = 2; // Jouer "Twinkle, Twinkle, Little Star"
      currentNote = 0;
      noteStartTime = millis();
      ledStartTime = millis();
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
    }
  }

  // Jouer la mélodie si elle est en cours
  if (isPlayingMelody) {
    unsigned long currentTime = millis();
    int melodyDuration = (activeMelody == 1) ? victoryDurations[currentNote] : twinkleDurations[currentNote];
    if (currentTime - noteStartTime >= melodyDuration) {
      // Passer à la note suivante
      currentNote++;
      if (activeMelody == 1 && currentNote < sizeof(victoryMelody) / sizeof(victoryMelody[0])) { // Mélodie Victory
        tone(buzzerPin, victoryMelody[currentNote], melodyDuration);
        noteStartTime = currentTime;
      } else if (activeMelody == 2 && currentNote < sizeof(twinkleMelody) / sizeof(twinkleMelody[0])) { // Mélodie Twinkle
        tone(buzzerPin, twinkleMelody[currentNote], melodyDuration);
        noteStartTime = currentTime;
      } else {
        // Fin de la mélodie
        noTone(buzzerPin);
        isPlayingMelody = false;
      }
    }
  }

  // Vérifier si 5 secondes se sont écoulées après l'allumage de la LED
  if ((button1Pressed || button2Pressed) && millis() - ledStartTime >= 5000) {
    // Éteindre les LEDs
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);

    // Clignoter les LEDs 5 fois
    blinkLedsFiveTimes();

    // Réinitialiser les états des boutons
    button1Pressed = false;
    button2Pressed = false;
  }
}
