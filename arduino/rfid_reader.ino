/*
 * Lecteur RFID Simple - Envoi UID via Série
 * 
 * Matériel requis :
 * - Arduino Uno/Mega
 * - Lecteur RFID MFRC522
 * - LCD 16x2 I2C (optionnel pour affichage local)
 * 
 * Connexions MFRC522 :
 *   SDA  -> Pin 10
 *   SCK  -> Pin 13
 *   MOSI -> Pin 11
 *   MISO -> Pin 12
 *   RST  -> Pin 9
 *   3.3V -> 3.3V
 *   GND  -> GND
 * 
 * Connexions LCD I2C (optionnel) :
 *   SDA -> A4
 *   SCL -> A5
 *   VCC -> 5V
 *   GND -> GND
 * 
 * Bibliothèques nécessaires :
 *   - MFRC522 by GithubCommunity
 *   - LiquidCrystal I2C by Frank de Brabander (si LCD utilisé)
 */

#include <SPI.h>
#include <MFRC522.h>

// Décommentez si vous utilisez un LCD
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// Configuration des pins
#define SS_PIN 10
#define RST_PIN 9

// Initialisation du lecteur RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Initialisation du LCD (décommentez si utilisé)
// LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresse 0x27, essayez 0x3F si ça ne marche pas

// Variables
String lastUID = "";
unsigned long lastScanTime = 0;
const unsigned long SCAN_DELAY = 2000; // Délai anti-rebond (2 secondes)

void setup() {
  // Initialisation série
  Serial.begin(9600);
  while (!Serial);
  
  // Initialisation SPI pour RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialisation LCD (décommentez si utilisé)
  // lcd.init();
  // lcd.backlight();
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Systeme RFID");
  // lcd.setCursor(0, 1);
  // lcd.print("Pret...");
  // delay(2000);
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Approchez carte");
  
  Serial.println("SYSTEM:READY");
  Serial.println("Lecteur RFID initialise - Approchez une carte");
}

void loop() {
  // Vérifier si une nouvelle carte est présente
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Lire la carte
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Construire l'UID
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uid += "0";
    }
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  // Anti-rebond : ignorer si même carte scannée récemment
  unsigned long currentTime = millis();
  if (uid == lastUID && (currentTime - lastScanTime) < SCAN_DELAY) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }

  // Nouvelle carte détectée
  lastUID = uid;
  lastScanTime = currentTime;

  // Envoyer l'UID via série
  Serial.print("RFID:");
  Serial.println(uid);

  // Afficher sur LCD (décommentez si utilisé)
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Carte detectee!");
  // lcd.setCursor(0, 1);
  // lcd.print(uid.substring(0, 16)); // Max 16 caractères

  // Arrêter la communication avec la carte
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(100);
}
