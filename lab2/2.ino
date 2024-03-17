// Плата одновременно работает в режиме сервера, и в режиме клиента.
// Сервер ожидает GET-запрос /on или /off. Клиент при нажатии на кнопку 
// их отправляет (с чередованием).
// Этот код нужно загрузить на обе платы, отличие будет в ip сервера.
//
// Если нет второй платы, а проверить корректность работы нужно, нужно установить следующее ПО (галочки по умолчанию):
// 1) Wireshark - для мониторинга сети,
// 2) XAMPP - для поднятия минимально рабочего Apache-сервера на компьютере,
// 3) Postman - для отправки GET-запросов с приятным GUI (требуется регистрация)
// или можно это сделать через терминальный cUrl (обычно уже установлен).
//
// В Wireshark выбираем нужную сеть из списка (вероятно, это будет "Беспроводная сеть").
// В окошко фильтра вставляем следующую строку "ip.addr==192.168.1.107 && http", замените ip-адрес на адрес платы,
// который можно узнать из Монитора Порта, загрузив код на плату. Теперь вы будете видеть все HTTP-запросы, которые
// плата отправляет или получает.
//
// В XAMPP нажимаем кнопку Start, что соответсвует пункту Apache. Теперь сервер запущен. IP-адрес можете узнать через
// команду ipcofig. С нажатием кнопки flash сервер будет молча принимать запросы, но это будет отображаться в Wireshark.
//
// Для отправки GET-запроса в интерфейсе Postman нажмите на плюсик и введите http://192.168.1.107:80/on или http://192.168.1.107:80/off.
// В случае с curl введите в терминал curl http://192.168.1.107:80/on. На выведенные ошибки осбого внимания не обращаем, это не помешает работе.

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Name";  // Your Wi-Fi Name
const char* password = "Password";   // Wi-Fi Password
const char* serverIP = "192.168.1.105"; // IP адрес сервера
const int LED = 2;
const int BUTTON = 0;
bool ledState = false;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.print("Connecting to the Network");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected"); 

  server.begin();
  Serial.println("Server started");

  Serial.print("Your IP Address: ");
  Serial.println(WiFi.localIP());
}

void handleServer() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.print("Received Request: ");
        Serial.println(request);
        if (request.indexOf("/off") != -1) { // '/off' для выключения
          digitalWrite(LED, HIGH); // Выключаем светодиод
        }
        else if (request.indexOf("/on") != -1) { // '/on' для включения
          digitalWrite(LED, LOW); // Включаем светодиод
        }
        break;
      }
    }
    client.stop();
  }
}

void sendToggleRequest() {
  Serial.println("Sending Toggle Request");
  HTTPClient http;
  WiFiClient client = server.available();
  String requestPath = ledState ? "/off" : "/on";
  ledState = !ledState;
  http.begin(client,"http://" + String(serverIP) + ":80" + requestPath);
  int httpCode = http.GET();

  if (httpCode > 0)
    Serial.printf("[HTTP] GET code: %d\n", httpCode);
  else
    Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());

  http.end();
}


void handleClient() {
  if (digitalRead(BUTTON) == LOW) {
    sendToggleRequest();
    delay(1000);  // Дополнительная задержка для предотвращения дребезга кнопки
  }
}


void loop() {
  handleServer();
  handleClient();
}
