#define BUZZER 7
#define LED_PIN 13

void setup() {
  Serial.begin(9600);
  pinMode(A5, INPUT);             // input sensor cahaya
  pinMode(LED_PIN, OUTPUT);   
  pinMode(BUZZER, OUTPUT); 
}

// LED berkedip
void soundAlert(int speed, int freq){
  tone(BUZZER, freq);
  delay(speed);
  noTone(BUZZER);
}

// sound buzzer di module
void loop() {
  int sensor_value = analogRead(A5);
  int adjusted_value = 1023 - sensor_value;  // Membalik nilai pembacaan
  Serial.println(adjusted_value);

  if (adjusted_value < 900) {  // Kondisi sesuai pembacaan cahaya terang
    digitalWrite(LED_PIN, HIGH);
    soundAlert(500, 1000);  // Buzzer menyala
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
