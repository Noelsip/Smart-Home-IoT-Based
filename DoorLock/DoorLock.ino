#include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
// LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "SPI.h"
#include "MFRC522.h"

#include <Servo.h>

Servo myservo;

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

// Variable untuk melacak status locked
bool isLocked = true;

// Daftar kode kartu yang valid
const String validCards[] = {
  "EF:D0:14:1E",
  "B1:62:DF:CC"
};
const int totalCards = sizeof(validCards) / sizeof(validCards[0]);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  myservo.attach(3);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("I am waiting for card...");
  // lcd.begin();
  myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
    // lcd.clear();
    // lcd.print("Tap your Card");
   
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // // Check is the PICC of Classic MIFARE type
  // if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
  //     piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
  //     piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
  //   Serial.println(F("Your tag is not of type MIFARE Classic."));
  //   return;
  // }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);

// pengecekan status kartu
  bool isValid = false;
  for (int i = 0; i < totalCards; i++){
    if (strID == validCards[i]){
      isValid = true;
      break;
    }
  }

  if (isValid){

    // Buzzer setting
    for (int i = 0; i < 2; i++){
      digitalWrite(BUZZER_PIN, HIGH);
      delay(40);
      digitalWrite(BUZZER_PIN, LOW);
      delay(40);
    }
    Serial.println("********************");
    Serial.println("**Authorised acces**");
    Serial.println("********************");

    if(isLocked){ // jika pintu terkunci buka pintu
      Serial.println("Unlocking door.....");
      myservo.write(90);
      isLocked = false; // status pintu berubah menjadi terbuka
    }
    else{ // jika pintu terbuka, kunci pintu
      Serial.println("Locking door...");
      myservo.write(0);
      isLocked = true; // status pintu berubah menjadi terkunci
    }
    delay(500);
    

  }
  else {
    Serial.println("****************");
    Serial.println("**Acces denied**");
    Serial.println("****************");
    Serial.println("**Invalid Card**");
    Serial.println("****************");
    // lcd.clear();
    // lcd.print("Kartu Tk Cocok");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
  }
}