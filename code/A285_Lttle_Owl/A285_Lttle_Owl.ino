/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
  Funktion: Liitle Owl A255

**************************************************************************************************
  Version: 02.11.2025
  ---------------------------------------------------------------
*************************************************************************************************
  Board: NANO 328P Old Bootloader
**************************************************************************************************
  C++ Arduino IDE V1.8.19
**************************************************************************************************
  Einstellungen:
  https://dl.espressif.com/dl/package_esp32_index.json
  http://dan.drown.org/stm32duino/package_STM32duino_index.json
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json


  ### Electronik Verbindungen
  Bauteil  Arduino-Pin  Beschreibung
  HC-SR04   VCC 5 V     Stromversorgung
  HC-SR04   GND GND     Masse
  HC-SR04   TRIG  D2    Trigger-Pin
  HC-SR04   ECHO  D3    Echo-Pin
  Servo 1   Signal  D9  Linker Flügel
  Servo 2   Signal  D10 Rechter Flügel
  Servos    VCC         Externe 5 V (nicht direkt über Nano speisen!)
  Servos    GND  GND    (gemeinsam mit Nano)  Masse

 **************************************************************************************************/

#include <Servo.h>

// Pins für den Ultraschallsensor
const int trigPin = 2;
const int echoPin = 3;

// Servos
Servo servoLeft;
Servo servoRight;
const int servoLeftPin = 9;
const int servoRightPin = 10;

// Abstandsschwelle
const int minDistance = 5;  // Untere Grenze in cm
const int maxDistance = 7;  // Obere Grenze in cm

// Variablen
long duration;
int distance;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);

  // Startposition der Flügel
  servoLeft.write(90);
  servoRight.write(90);
}

void loop() {
  distance = getDistance();
  Serial.print("Abstand: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wenn Abstand zwischen 5 und 7 cm
  if (distance >= minDistance && distance <= maxDistance) {
    flapWings();  // Flügel bewegen
  } else {
    // Ruheposition
    servoLeft.write(90);
    servoRight.write(90);
  }

  delay(200);
}

// --- Funktion zur Abstandsmessung ---
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;  // Schallgeschwindigkeit / 2
  return distance;
}

// --- Flügelschlagfunktion ---
void flapWings() {
  for (int i = 0; i < 7; i++) { // 7 Flügelschläge
    servoLeft.write(50);   // linker Flügel runter
    servoRight.write(130); // rechter Flügel runter
    delay(250);

    servoLeft.write(130);  // linker Flügel hoch
    servoRight.write(50);  // rechter Flügel hoch
    delay(250);
  }

  // Nach dem Schlagen: Zur Ruheposition zurück
  servoLeft.write(90);
  servoRight.write(90);
}
