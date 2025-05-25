#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// pin yang digunakan
// ultrasonik sensor ketinggian air
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

// variabel global untuk menyimpan ketinggian air sebelumnya
float lastWaterLevel = -1;

bool wasInDanger = false; // status air sebelumnya

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
  
  // perbandingan antara ketinggian air sekarang dengan sebelumnya
  //untuk mengurangi spam
  float diff = abs(water - lastWaterLevel);

  // Kondisi BAHAYA: air meluap <= 10cm dari sensor
  if (water <= 10) {
    if ((!wasInDanger || diff >= 5) && lastWaterLevel >= 0) {
      String message = "WASPADA!! Ketinggian air ";
      message += String(water, 2);
      message += " cm dari permukaan tanah.";
      bot.sendMessage(grup_id, message, ""); // kirim pesan ke grup telegram
    }
    wasInDanger = true;
  }
  // Kondisi NORMAL: air surut > 10cm
  else {
    if (wasInDanger && water > 10) {
      String message = "Ketinggian air telah kembali normal: ";
      message += String(water, 2);SSSSSS
      message += " cm dari permukaan tanah.";
      bot.sendMessage(grup_id, message, "");
    }
    wasInDanger = false;
  }

  lastWaterLevel = water;
  delay(1000);

}
