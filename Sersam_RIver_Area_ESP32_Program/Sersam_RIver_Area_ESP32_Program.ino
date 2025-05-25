#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// pin yang digunakan
// ultrasonik sensorn ketinggian air
#define trigPinWater 5
#define echoPinWater 18

// Konfigurasi Wifi dan Telegram
const char* ssid = "Kos ijo";
const char* password = "Aslan199";
const char* botToken = "7918196245:AAHZPzSlKy9jYdkK-L5sHdxCc3hvg9O54Ik";
const char* grup_id = "-4721702872";

// inisialisasi library 
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// perhitungan sensor ultrasonik untuk ketinggian air
float WaterDistance(){
  digitalWrite(trigPinWater, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinWater, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinWater, LOW);
  
  // rumusnya
  long duration = pulseIn(echoPinWater, HIGH);
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  // ultrasonik sensorn ketinggian air
  pinMode(trigPinWater, OUTPUT);
  pinMode(echoPinWater, INPUT);
  
  // Menyambungkan WiFI
  WiFi.begin(ssid, password);
  client.setInsecure();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Wifi Terhubung!");
}

void loop() {
  float water = WaterDistance(); // ketinggian air
  Serial.print("ketinggian air: ");
  Serial.print(water);
  Serial.println(" cm");
  
  // kondisi ketika air meluap
  if (water <= 10){
    String message = "WASPADA!! ketinggian air ";
    message += String(water); // Mengonversi float ke String
    message += " cm dari permukaan tanah";
    bot.sendMessage(grup_id, message, ""); //kirim pesan ke grup telegram melalui grup id
  }
  delay(1000);
}