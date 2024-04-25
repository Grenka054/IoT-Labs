#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>

#define SERVER_URL "http://192.168.1.108/check_update.php"

// Глобальная переменная для хранения текущей версии прошивки
String current_version = "firmware_v1.0.0.ino"; // Замените на вашу текущую версию прошивки

unsigned long lastCheckTime = 0;
unsigned long lastBlinkTime = 0;

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin("login", "password");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    unsigned long currentMillis = millis();

    // Blink once every 2 sec
    if (currentMillis - lastBlinkTime >= 2000) {
        lastBlinkTime = currentMillis;
        digitalWrite(LED_BUILTIN, LOW);
        delay(300);
        digitalWrite(LED_BUILTIN, HIGH);
    }

    // Check for updates every 30 sec
    if (currentMillis - lastCheckTime >= 30000) {
        lastCheckTime = currentMillis;

      // Send a GET request to the server
      HTTPClient http;
      WiFiClient client;
      http.begin(client, SERVER_URL);
      int httpCode = http.GET();
      if (httpCode == 200) {
          // Parse the response
          String payload = http.getString();
          DynamicJsonDocument doc(1024);
          deserializeJson(doc, payload);
          String latest_version = doc["version"];
          String firmware_url = doc["url"];

          // If a new version is available, perform an OTA update
          if (latest_version != current_version) {
              Serial.println("New firmware version available, updating...");
              t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmware_url, latest_version);
              switch (ret) {
                  case HTTP_UPDATE_FAILED:
                      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                      break;
                  case HTTP_UPDATE_NO_UPDATES:
                      Serial.println("HTTP_UPDATE_NO_UPDATES");
                      break;
                  case HTTP_UPDATE_OK:
                      Serial.println("HTTP_UPDATE_OK");
                      current_version = latest_version; // Обновите текущую версию после успешного обновления
                      break;
              }
          } else {
              Serial.println("No new firmware version available");
          }
      } else {
          Serial.printf("GET request failed, error: %s", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
}
