// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h>

// // pin yang digunakan
// // ultrasonik sensor ketinggian air
// #define trigPinWater 5
// #define echoPinWater 18

// // jsn jarak sampah
// #define trigPinDist 22
// #define echoPinDist 23

// // stepper motor
// #define DIR  12
// #define STEP 13

// // const jarak sampah 
// #define JARAK_SAMPAH_MAKS 30

// // Konfigurasi Wifi dan Telegram
// const char* ssid = "Kos ijo";
// const char* password = "Aslan199";
// const char* botToken = "7918196245:AAHZPzSlKy9jYdkK-L5sHdxCc3hvg9O54Ik";
// const char* grup_id = "-4721702872";

// // inisialisasi library 
// WiFiClientSecure client;
// UniversalTelegramBot bot(botToken, client);

// // variabel global untuk menyimpan ketinggian air sebelumnya
// float lastWaterLevel = -1;

// bool wasInDanger = false; // status air sebelumnya

// // Inisialisasi stepper (tipe DRIVER: hanya STEP & DIR)
// // AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR);

// // Variabel waktu untuk logika delay berhenti motor
// unsigned long waktuTerakhirDekat = 0;
// const unsigned long delayBerhenti = 2000;

// // perhitungan sensor ultrasonik untuk ketinggian air
// float WaterDistance(){
//   digitalWrite(trigPinWater, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPinWater, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPinWater, LOW);
  
//   // rumusnya
//   long duration = pulseIn(echoPinWater, HIGH);
//   return duration * 0.034 / 2;
// }

// // Fungsi baca jarak ultrasonik (satu kali pembacaan)
// float readDistance() {
//   digitalWrite(trigPinDist, LOW);            // Bersihkan sinyal trigger
//   delayMicroseconds(2);
//   digitalWrite(trigPinDist, HIGH);           // Kirim pulsa trigger 10 mikrodetik
//   delayMicroseconds(10);
//   digitalWrite(trigPinDist, LOW);

//   // Baca durasi pantulan echo (maks 30ms untuk timeout)
//   int duration = pulseIn(echoPinDist, HIGH, 30000); 

//   // Jika ada hasil, konversi ke cm, jika tidak return -1
//   return duration > 0 ? duration * 0.034 / 2 : -1;
// }

// // Fungsi filter jarak (rata-rata beberapa pembacaan)
// float getFilteredDistance(int n = 5) {
//   float total = 0;
//   int count = 0;

//   // Ambil n pembacaan dan hitung rata-rata
//   for (int i = 0; i < n; i++) {
//     float d = readDistance();
//     if (d > 0) {
//       total += d;
//       count++;
//     }
//     delay(10);  // Delay singkat antar pembacaan
//   }

//   return (count > 0) ? total / count : -1;  // Hindari pembagian nol
// }

// void setup() {
//   Serial.begin(115200);

//   // ultrasonik sensorn ketinggian air
//   pinMode(trigPinWater, OUTPUT);
//   pinMode(echoPinWater, INPUT);

//   // jsn untuk jarak sampah
//   pinMode(trigPinDist, OUTPUT);         
//   pinMode(echoPinDist, INPUT);           
  
//   // Menyambungkan WiFI
//   WiFi.begin(ssid, password);
//   client.setInsecure();
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.print("Wifi Terhubung!");
// }

// void loop() {
//   // -----------------------------
//   // Bagian 1: Pengukuran Jarak Sampah & Kendali Stepper
//   // -----------------------------
//   float distance = getFilteredDistance();         // Jarak ke sampah
//   unsigned long sekarang = millis();              // Waktu sekarang (ms)

//   if (distance <= 200 && distance > 0) {
//     waktuTerakhirDekat = sekarang;                // Perbarui waktu deteksi terakhir
//     stepper.setSpeed(500);                        // Konveyor jalan
//   } else {
//     if (sekarang - waktuTerakhirDekat < delayBerhenti) {
//       stepper.setSpeed(500);                      // Masih dalam jeda: jalan terus
//     } else {
//       stepper.setSpeed(0);                        // Lewat jeda: berhenti
//     }
//   }

//   stepper.runSpeed();  // WAJIB dipanggil di setiap loop untuk motor jalan

//   // -----------------------------
//   // Bagian 2: Monitoring Ketinggian Air & Kirim Telegram
//   // -----------------------------
//   float water = WaterDistance();  // Dapatkan ketinggian air
//   Serial.print("ketinggian air: ");
//   Serial.print(water);
//   Serial.println(" cm");

//   float diff = abs(water - lastWaterLevel);  // Selisih terhadap data sebelumnya

//   // Jika air sangat tinggi (<=10 cm ke sensor)
//   if (water <= 10) {
//     if ((!wasInDanger || diff >= 5) && lastWaterLevel >= 0) {
//       String message = "WASPADA!! Ketinggian air ";
//       message += String(water, 2);
//       message += " cm dari permukaan tanah.";
//       bot.sendMessage(grup_id, message, "");
//     }
//     wasInDanger = true;
//   } 
//   // Jika air kembali normal (>10 cm)
//   else {
//     if (wasInDanger && water > 10) {
//       String message = "Ketinggian air telah kembali normal: ";
//       message += String(water, 2);
//       message += " cm dari permukaan tanah.";
//       bot.sendMessage(grup_id, message, "");
//     }
//     wasInDanger = false;
//   }

//   lastWaterLevel = water;

//   delay(1000);  // Delay kecil, cukup aman untuk aplikasi ini
// }


// INI KODE TANPA STEPPER MOTOR


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <UniversalTelegramBot.h>

// ---------------------- Konstanta Pin ----------------------
#define trigPinWater 5     // Pin trigger sensor air
#define echoPinWater 18    // Pin echo sensor air

#define trigPinDist 22     // Pin trigger sensor jarak sampah
#define echoPinDist 23     // Pin echo sensor jarak sampah

#define trigPinTrash 19     // Pin trigger sensor kapasitas sampah
#define echoPinTrash 21     // Pin echo sensor kapasitas sampah

// ---------------------- Konfigurasi WiFi & API ----------------------
const char* ssid      = "Kos ijo";
const char* password  = "Aslan199";
const char* botToken  = "7918196245:AAHZPzSlKy9jYdkK-L5sHdxCc3hvg9O54Ik";
const char* grup_id   = "-4721702872";
const char* server    = "http://192.168.1.3/Sersam-EIS/WebMonitoring/api/receive.php";

// ---------------------- Objek & Variabel Global ----------------------
HTTPClient http;
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

float lastWaterLevel = -1;
bool wasInDanger = false;
float trashLevel = 10.39;

// ---------------------- Fungsi Sensor ----------------------

// Mengukur ketinggian air
float WaterDistance() {
  digitalWrite(trigPinWater, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinWater, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinWater, LOW);

  long duration = pulseIn(echoPinWater, HIGH);
  return duration * 0.034 / 2;
}

// Mengukur jarak sampah (sekali baca)
float readDistance() {
  digitalWrite(trigPinDist, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDist, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDist, LOW);

  float duration = pulseIn(echoPinDist, HIGH, 30000);
  return (duration > 0) ? duration * 0.034 / 2 : -1;
}

// Mengukur jarak sampah (sekali baca)
float trashDistance() {
  digitalWrite(trigPinTrash, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinTrash, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinTrash, LOW);

  float duration = pulseIn(echoPinTrash, HIGH, 30000);
  return (duration > 0) ? duration * 0.034 / 2 : -1;
}

// Rata-rata pembacaan jarak sampah
float getFilteredDistance(int n = 5) {
  float total = 0;
  int count = 0;

  for (int i = 0; i < n; i++) {
    float d = readDistance();
    if (d > 0) {
      total += d;
      count++;
    }
    delay(10);
  }
  return (count > 0) ? total / count : -1;
}

// Rata-rata pembacaan jarak sampah
float getFilteredTrashDistance(int n = 5) {
  float total = 0;
  int count = 0;

  for (int i = 0; i < n; i++) {
    float d = trashDistance();
    if (d > 0) {
      total += d;
      count++;
    }
    delay(10);
  }
  return (count > 0) ? total / count : -1;
}

// Kirim data ke server
void sendData(float waterLevel, float trashLevel) {
  http.begin(server);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "ketinggianair=" + String(waterLevel, 2) + "&kapasitassampah=" + String(trashLevel, 2);
  int httpCode = http.POST(postData);

  if (httpCode > 0) {
    Serial.printf("[SERVER] Response: %d\n", httpCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.printf("[SERVER] Failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

// ---------------------- Setup ----------------------
void setup() {
  Serial.begin(115200);

  pinMode(trigPinWater, OUTPUT);
  pinMode(echoPinWater, INPUT);
  pinMode(trigPinDist, OUTPUT);
  pinMode(echoPinDist, INPUT);
  pinMode(trigPinTrash, OUTPUT);
  pinMode(echoPinTrash, INPUT);

  WiFi.begin(ssid, password);
  client.setInsecure(); // Untuk koneksi HTTPS Telegram

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung!");
}

// ---------------------- Loop ----------------------
void loop() {
  float distanceLevel = getFilteredDistance();
  float waterLevel = WaterDistance();
  float trashLevel = getFilteredTrashDistance();
  float diff = abs(waterLevel - lastWaterLevel);

  // Output ke Serial Monitor
  Serial.print("Jarak sampah: ");
  Serial.print(distanceLevel);
  Serial.println(" cm");

  Serial.print("Ketinggian air: ");
  Serial.print(waterLevel);
  Serial.println(" cm");
 
  Serial.print("Kapasitas sampah: ");
  Serial.print(trashLevel);
  Serial.println(" cm");

  // Logika peringatan air tinggi
  if (waterLevel <= 10) {
    if ((!wasInDanger || diff >= 5) && lastWaterLevel >= 0) {
      String message = "WASPADA!! Ketinggian air ";
      message += String(waterLevel, 2);
      message += " cm dari permukaan tanah.";
      bot.sendMessage(grup_id, message, "");
    }
    wasInDanger = true;
  } else {
    if (wasInDanger && waterLevel > 10) {
      String message = "Ketinggian air telah kembali normal: ";
      message += String(waterLevel, 2);
      message += " cm dari permukaan tanah.";
      bot.sendMessage(grup_id, message, "");
    }
    wasInDanger = false;
  }

  sendData(waterLevel, trashLevel);
  lastWaterLevel = waterLevel;

  delay(1000);  // Delay antar pembacaan
}
