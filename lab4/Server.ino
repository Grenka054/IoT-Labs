// 1 плата и ПК
// Плата-сервер
// 1. На плату загрузить серверный код, скопировать адрес для подключения.
// 2. Обращаться по этому адресу через Postman, например, http://192.168.1.103:80/state.
// Внизу будет отображаться полученный JSON.


#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Name";  // Your Wi-Fi Name
const char* password = "Password";   // Wi-Fi Password

#define RED_LED D0
#define YELLOW_LED D1
#define GREEN_LED D2

struct State {
  int red;
  int yellow;
  int green;
};

State states[30] = {
  {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, // Зеленый горит
  {0, 0, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0, 1}, // Зеленый мерцает
  {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, // Желтый горит
  {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, // Красный горит
  {1, 1, 0}, {1, 1, 0}, {1, 1, 0} // Горят красный и жёлтый
};

AsyncWebServer server(80);

int lastStateIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected"); 

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    int stateIndex = millis() / 1000 % 30; // Переключение состояний каждую секунду
    stateIndex = (stateIndex + 15) % 30; // Сдвиг, чтобы состояния двух светофоров были противоположны
    String json = "{";
    json += "\"green\":" + String(states[stateIndex].green) + ",";
    json += "\"yellow\":" + String(states[stateIndex].yellow) + ",";
    json += "\"red\":" + String(states[stateIndex].red);
    json += "}";

    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  int stateIndex = millis() / 1000 % 30; // Переключение состояний каждую секунду
  State state = states[stateIndex];
  if (stateIndex != lastStateIndex)
    Serial.printf("Red: %d, Yellow: %d, Green: %d\n", state.red, state.yellow, state.green);
    lastStateIndex = stateIndex;
  digitalWrite(GREEN_LED, state.green);
  digitalWrite(YELLOW_LED, state.yellow);
  digitalWrite(RED_LED, state.red);
}
