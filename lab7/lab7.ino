#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// WiFi
const char* ssid = "Name";
const char* password = "Password";

// MQTT Broker
const char *mqtt_broker = "mqtt.by";
const char *topic = "user/login/trafficligth";
const char *mqtt_username = "login";
const char *mqtt_password = "password";
const int qos = 0; // QoS 2 не поддерживается

#define DHTPIN 2     // Цифровой пин, подключенный к датчику DHT
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// Текущая температура и влажность, обновляются в loop()
float t = 0.0;
float h = 0.0;

// Общий интервал между чтениями с DHT
const long interval = 10000;  
unsigned long previousMillis = 0;    // последнее время обновления DHT

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_broker, 1883);
  dht.begin();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

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
      client.publish("user/grenka222/temperature", String(t).c_str(), true);
    }
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.printf("humidity: %f\n",h);
      client.publish("user/grenka222/humidity", String(h).c_str(), true);
    }
  }
}