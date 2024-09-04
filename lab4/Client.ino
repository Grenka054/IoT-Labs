// 1 плата и ПК
// Плата-клиент
// 1. Узнать ip компьютера с помощью ipconfig. Этот адрес вставить в клиентский код,
// раскомментировать 47 строку,залить на плату.
// 2. Установить XAMPP. В папку ...\xampp\htdocs скопировать php-файлы. Запустить Apache-сервер.
// 3. В браузере открыть http://localhost/state_visual.php. Это визуализация серверного 
// светофора (не обновялется при остуствии GET-запросов, но можно спамить через Runner в интерфейсе Postman).

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Name";  // Your Wi-Fi Name
const char* password = "Password";   // Wi-Fi Password
const char* host = "192.168.1.108";  // Host IP Address

#define RED_LED D0
#define YELLOW_LED D1
#define GREEN_LED D2

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    WiFiClient client;

    // Выберите куда отправлять запрос в зависимости от задачи.
    // http.begin(client, String("http://") + host + "/state.php"); // Запрос на пк
    http.begin(client, String("http://") + host + "/state"); // Запрос на esp8266

    int httpCode = http.GET();  //Send the request

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      int red = doc["red"];
      int yellow = doc["yellow"];
      int green = doc["green"];

      digitalWrite(RED_LED, red);
      digitalWrite(YELLOW_LED, yellow);
      digitalWrite(GREEN_LED, green);
    }
    else {
      Serial.println("Request received no response"); 
    }

    http.end();   //Close connection
  }

  delay(500);    //Send a request every second
}
