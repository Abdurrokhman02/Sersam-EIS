#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <SERSAMOTA.h>

// -----------------Sensor & aktuator Pin-------------------------
#define trigPinWater 5
#define echoPinWater 18
#define trigPinDist 22
#define echoPinDist 23
#define trigPinTrash 19
#define echoPinTrash 21
#define STEP_PIN 13
#define DIR_PIN 12
#define ENABLE_PIN 14
#define MICROSTEPPING 4
#define STEPS_PER_REV 200

// ----------------------------Connection--------------------------

const char* ssid = "Esthe";
const char* password = "RJSPQLBS";

// -----------------------------Server dan Telegram---------------
const char* botToken = "7918196245:AAHZPzSlKy9jYdkK-L5sHdxCc3hvg9O54Ik";
const char* grup_id = "-4721702872";
const char* server = "https://abdurrokhman.my.id/Sersam/api/receive.php";

// -----------------------------Objek------------------------------
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
HTTPClient http;
SERSAMOTA ota("SERSAM", 8080);

// -----------------------------Global-----------------------------
// PENTING: Gunakan 'volatile' untuk variabel yang diakses oleh banyak task/core
volatile float waterLevel = -1, trashLevel = -1, distanceLevel = -1, lastWaterLevel = -1;
volatile bool wasInDanger = false;
volatile unsigned long waktuTerakhirDekat = 0;
const unsigned long delayBerhenti = 2000;

// ------------------ BACA ULTRASONIK -----------------------------
float ultrasonicRead(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  float duration = pulseIn(echo, HIGH, 30000); // 30ms timeout
  return (duration > 0) ? duration * 0.034 / 2 : -1;
}

int calculateStepDelay(int rpm) {
  float step_frequency = (rpm / 60.0) * STEPS_PER_REV * MICROSTEPPING;
  return 1000000 / step_frequency;
}

void stepMotor(int steps, int rpm, bool clockwise = true) {
  digitalWrite(ENABLE_PIN, LOW);       // Aktifkan driver
  digitalWrite(DIR_PIN, clockwise ? HIGH : LOW);   // Arah putaran
  int delayMicros = calculateStepDelay(rpm);

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(delayMicros);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(delayMicros);
  }
}


// ------------------ TASK SENSOR -----------------------------
void taskSensor(void *param) {
  while (1) {
    // Membaca nilai sensor dan menyimpannya ke variabel volatile
    waterLevel = ultrasonicRead(trigPinWater, echoPinWater);
    distanceLevel = ultrasonicRead(trigPinDist, echoPinDist);
    trashLevel = ultrasonicRead(trigPinTrash, echoPinTrash);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ------------------ TASK STEPPER ---------------------------
void taskStepper(void *param) {
  while (1) {
    unsigned long sekarang = millis();

    // Membaca nilai dari variabel volatile
    if (distanceLevel <= 50 && distanceLevel > 0) {
      waktuTerakhirDekat = sekarang;
      stepMotor(50, 60); // contoh: 50 langkah dengan 60 RPM
    } else {
      if (sekarang - waktuTerakhirDekat < delayBerhenti) {
        stepMotor(20, 30); // gerak pelan sebentar
      }
      // Jika sudah lewat waktu, tidak gerak sama sekali
    }

    vTaskDelay(10 / 
    );
  }
}

// ------------------ TASK KOMUNIKASI -----------------------
void taskKomunikasi(void *param) {
  while (1) {
    // Membaca nilai dari variabel volatile
    float diff = abs(waterLevel - lastWaterLevel);

    Serial.printf("Jarak sampah: %.2f cm\n", distanceLevel);
    Serial.printf("Ketinggian air: %.2f cm\n", waterLevel);
    Serial.printf("Kapasitas sampah: %.2f cm\n", trashLevel);

    if (waterLevel <= 10 && waterLevel > 0) { // Menambahkan pengecekan > 0 untuk data valid
      if ((!wasInDanger || diff >= 5) && lastWaterLevel >= 0) {
        String message = "WASPADA!! Ketinggian air " + String(waterLevel, 2) + " cm dari permukaan tanah.";
        bot.sendMessage(grup_id, message, "");
      }
      wasInDanger = true;
    } else {
      if (wasInDanger && waterLevel > 10) {
        String message = "Ketinggian air telah kembali normal: " + String(waterLevel, 2) + " cm.";
        bot.sendMessage(grup_id, message, "");
      }
      wasInDanger = false;
    }

    // Hanya kirim data jika valid (bukan -1)
    if (waterLevel > 0 && trashLevel > 0) {
        http.begin(server);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String postData = "ketinggianair=" + String(waterLevel, 2) + "&kapasitassampah=" + String(trashLevel, 2);
        int httpCode = http.POST(postData);
        if (httpCode > 0) {
          Serial.printf("[SERVER] Response: %d\n", httpCode);
          Serial.println(http.getString());
        } else {
          Serial.printf("[SERVER] Failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }

    lastWaterLevel = waterLevel;
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

// ------------------ TASK OTA HANDLE -------------------------
void taskOTA(void *param) {
  while (1) {
    ota.handle();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// ------------------ SETUP -----------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(trigPinWater, OUTPUT);
  pinMode(echoPinWater, INPUT);
  pinMode(trigPinDist, OUTPUT);
  pinMode(echoPinDist, INPUT);
  pinMode(trigPinTrash, OUTPUT);
  pinMode(echoPinTrash, INPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH); // Mulai dengan menonaktifkan driver untuk hemat daya

  WiFi.begin(ssid, password);
  client.setInsecure();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");

  // OTA Configuration
  ota.setOTAPassword("sersam123");
  ota.enableSerialDebug(true);
  ota.enableMDNS(true);
  
  // (Fungsi callback OTA tetap sama)
  ota.onStart([]() { Serial.println("OTA Update Started!"); });
  ota.onEnd([]() { Serial.println("OTA Update Finished!"); });
  ota.onProgress([](unsigned int progress, unsigned int total) { Serial.printf("OTA Progress: %u%%\n", (progress * 100) / total); });
  ota.onError([](ota_error_t error) { Serial.printf("OTA Error: %u\n", error); });
  ota.onWiFiConnected([]() { Serial.println("WiFi Connected - OTA Ready!"); });
  ota.onWiFiDisconnected([]() { Serial.println("WiFi Disconnected - OTA Not Available"); });
  ota.onWebUpdateStart([]() { Serial.println("Web Update Started!"); });
  ota.onWebUpdateEnd([](bool success) { Serial.println(success ? "Web Update Successful!" : "Web Update Failed!"); });

  if (ota.begin(ssid, password)) {
    Serial.println("OTA Service Started Successfully!");
    Serial.println("Upload URL: " + ota.getUploadURL());
  } else {
    Serial.println("Failed to start OTA service!");
  }

  // FreeRTOS Tasks
  xTaskCreatePinnedToCore(taskSensor, "Sensor", 4096, NULL, 2, NULL, 1);       // Prioritas lebih tinggi
  xTaskCreatePinnedToCore(taskStepper, "Stepper", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskKomunikasi, "Komunikasi", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskOTA, "OTA", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Tidak perlu digunakan karena semua fungsi jalan di FreeRTOS
}
