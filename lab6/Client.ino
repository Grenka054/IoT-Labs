#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi
const char* ssid = "Name";
const char* password = "Password";

// MQTT Broker
const char *mqtt_broker = "mqtt.by";
const char *topic = "user/login/trafficlight";
const char *mqtt_username = "login";
const char *mqtt_password = "password";
const int qos = 1; // QoS 2 не поддерживается

#define RED_LED D0
#define YELLOW_LED D1
#define GREEN_LED D2

WiFiClient espClient;
PubSubClient client(espClient);

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

  client.setServer(mqtt_broker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  static byte lastPayload[512];
  static unsigned int lastLength = 0;

  // Если новое сообщение больше предыдущего, то обновляем буфер
  if (length > lastLength) {
    lastPayload = (byte*) realloc(lastPayload, length);
    lastLength = length;
  }

  // Копируем новое сообщение в буфер
  memcpy(lastPayload, payload, length);

  // Обрабатываем последнее сообщение
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload, length);

  int red = doc["red"];
  int yellow = doc["yellow"];
  int green = doc["green"];

  digitalWrite(RED_LED, red);
  digitalWrite(YELLOW_LED, yellow);
  digitalWrite(GREEN_LED, green);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(topic, qos);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(1000);
    }
  }
}