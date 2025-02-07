#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27,16,4);
// Deklarasi PIN DHT11 //
#define DHTPIN 2
// Tipe sensor yang digunakan (DHT11 atau DHT22) //
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Membuat ikon kelelembaban // 
byte suhu[8] =
{
  B00100,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110,
  B00000
};

// Membuat ikon kelelembaban // 
byte kelembaban[8] =
{
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
  B00000
};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, kelembaban);
  lcd.createChar(2, suhu);
  lcd.setCursor(0,0);
  lcd.print("Mahirelektro.com");
  lcd.setCursor(0,1);
  lcd.print("DHT11 + LCD 16x2");
  dht.begin();
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.print(" Suhu: ");
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.print(" Lembab: ");
}

void loop() {
  // Membaca kelembaban //
  float h = dht.readHumidity();
  // Membaca suhu dalam satuan Celsius //
  float t = dht.readTemperature();
  // Membaca suhu dalam satuan Fahrenheit //
  float f = dht.readTemperature(true);

  // Menampilkan pesan Error jika sensor tidak terbaca //
  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.setCursor(8,0);
    lcd.print("Error   ");
    lcd.setCursor(10,1);
    lcd.print("Error   ");
    return;
  }

  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  // Menampilkan data ke LCD //
  lcd.setCursor(8,0);
  lcd.print(t,1);
  lcd.print((char)223);
  lcd.print("C     ");
  lcd.setCursor(10,1);
  lcd.print(h,0);
  lcd.print("%     ");

  // Jeda Waktu, ubah menjadi 2000 untuk DHT22//
  delay(1000);
}