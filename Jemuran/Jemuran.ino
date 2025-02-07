#include <Servo.h>

#define WATER_SENSOR A0  // Sensor hujan di pin analog A0
#define LED_HIJAU 7
#define LED_MERAH 6
#define THRESHOLD 100    // Nilai batas sensor hujan (sesuaikan)

Servo jemuran;  // Objek servo

void setup() {
    Serial.begin(9600);
    jemuran.attach(9);  // Servo dihubungkan ke pin 9
    jemuran.write(0);   // Posisi awal jemuran
    pinMode(WATER_SENSOR, INPUT);
     pinMode(LED_MERAH, OUTPUT);
     pinMode(LED_HIJAU, OUTPUT);
}

void loop() {
    int sensorValue = analogRead(WATER_SENSOR);  // Baca nilai sensor
    Serial.println(sensorValue);

    if (sensorValue > THRESHOLD) {  
        jemuran.write(0);  // Putar servo untuk menutup jemuran
         digitalWrite(LED_HIJAU, LOW);
         digitalWrite(LED_MERAH, HIGH);
    } else {
        jemuran.write(90);  // Kembalikan ke posisi awal
         digitalWrite(LED_HIJAU, HIGH);
         digitalWrite(LED_MERAH, LOW);
    }
}
