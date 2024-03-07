// В Postman прописать GET-запросы (ip исправить):
// http://192.168.68.66:80/temperature
// http://192.168.68.66:80/humidity

// Import required libraries
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Name";    //  Ваше имя Wi-Fi
const char* password = "Password";   // Пароль Wi-Fi

#define DHTPIN 2     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // последнее время обновления DHT

// интервал между считываниями с DHT
const long interval = 10000;  


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  
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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"message\": \"Success!\"}");
  });

  // Маршрут для получения температуры
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    // Создаем JSON-объект
    StaticJsonDocument<100> jsonDoc;
    jsonDoc["temperature"] = t; // Записываем температуру в JSON
    String response;
    serializeJson(jsonDoc, response); // Преобразуем JSON в строку
    request->send(200, "application/json", response); // Отправляем JSON
  });

  // Маршрут для получения влажности
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    // Создаем JSON-объект
    StaticJsonDocument<100> jsonDoc;
    jsonDoc["humidity"] = h; // Записываем влажность в JSON
    String response;
    serializeJson(jsonDoc, response); // Преобразуем JSON в строку
    request->send(200, "application/json", response); // Отправляем JSON
  });

  // Start server
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // сохранить время последнего обновления с DHT
    previousMillis = currentMillis;
    // считать температуру в Цельсиях и влажность
    float newT = dht.readTemperature();
    float newH = dht.readHumidity();
    // обновление переменных в случае успешного считывания
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.printf("temperature: %f\n",t);
    }
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.printf("humidity: %f\n",h);
    }
  }
}
