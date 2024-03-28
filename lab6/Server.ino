#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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

WiFiClient espClient;
PubSubClient client(espClient);

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

  client.setServer(mqtt_broker, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int stateIndex = millis() / 1000 % 30; // Переключение состояний каждую секунду
  State state = states[stateIndex];
  stateIndex = (stateIndex + 15) % 30; // Сдвиг, чтобы состояния двух светофоров были противоположны

  Serial.printf("Red: %d, Yellow: %d, Green: %d\n", state.red, state.yellow, state.green);
  String json = "{";
  json += "\"green\":" + String(states[stateIndex].green) + ",";
  json += "\"yellow\":" + String(states[stateIndex].yellow) + ",";
  json += "\"red\":" + String(states[stateIndex].red);
  json += "}";
  client.publish(topic, json.c_str(), qos);

  digitalWrite(GREEN_LED, state.green);
  digitalWrite(YELLOW_LED, state.yellow);
  digitalWrite(RED_LED, state.red);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(1000);
    }
  }
}
