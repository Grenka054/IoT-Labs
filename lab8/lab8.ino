#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define FLASH_BUTTON_PIN 0 // GPIO0

//Variables
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;

//Function Declaration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void startSetupMode(void);

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  Serial.println("\nDisconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FLASH_BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Startup");

  //---------------------------------------- Read EEPROM for SSID and pass
  Serial.println("Reading EEPROM ssid and pass");

  String esid;
  for (int i = 0; i < 32; ++i)
    esid += char(EEPROM.read(i));

  String epass = "";
  for (int i = 32; i < 96; ++i)
    epass += char(EEPROM.read(i));

  Serial.print("SSID: ");
  Serial.println(esid);

  Serial.print("PASS: ");
  Serial.println(epass);

  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("\nSuccesfully Connected!!!");
    return;
  }
  Serial.println("\nTurning the HotSpot On");
  startSetupMode();
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);

  if (!digitalRead(FLASH_BUTTON_PIN)) {
    Serial.println("Button pressed. Entering setup mode...");
    startSetupMode();
  }
  delay(100);

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    for(int i = 0; i < 6; i++)
    {
      delay(500);
      if (!digitalRead(FLASH_BUTTON_PIN))
        return;
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);  
  }
}

void startSetupMode(void){
  //launchWeb();
  setupAP();

  Serial.print("\nWaiting.");

  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
  Serial.println("");
}

bool testWifi(void)
{
  Serial.println("Waiting for Wifi to connect");
  for (int c = 0; c < 20; c++) {
    if (WiFi.status() == WL_CONNECTED)
      return true;
    delay(500);
    Serial.print("*");
  }
  Serial.println("\nConnect timed out, opening AP");
  return false;
}

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  if (n == 0)
    Serial.println("No networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("ESP_DEFAULT", "123456789");
  Serial.println("Softap");
  launchWeb();
  Serial.println("Over");
}

void createWebServer()
{
  server.on("/", []() {

    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
    content += ipStr;
    content += "<p>";
    content += st;
    content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
    content += "</html>";
    server.send(200, "text/html", content);
  });
  server.on("/scan", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

    content = "<!DOCTYPE HTML>\r\n<html>go back";
    server.send(200, "text/html", content);
  });

  server.on("/setting", []() {
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0) {
      Serial.println("clearing eeprom");
      for (int i = 0; i < 96; ++i) {
        EEPROM.write(i, 0);
      }
      Serial.println(qsid);
      Serial.println("");
      Serial.println(qpass);
      Serial.println("");

      Serial.println("writing eeprom ssid:");
      for (int i = 0; i < qsid.length(); ++i)
      {
        EEPROM.write(i, qsid[i]);
        Serial.print("Wrote: ");
        Serial.print(qsid[i]);
      }
      Serial.println("");
      Serial.println("writing eeprom pass:");
      for (int i = 0; i < qpass.length(); ++i)
      {
        EEPROM.write(32 + i, qpass[i]);
        Serial.print("Wrote: ");
        Serial.print(qpass[i]);
      }
      EEPROM.commit();
      Serial.println("");
      content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
      statusCode = 200;
      ESP.reset();
    } else {
      content = "{\"Error\":\"404 not found\"}";
      statusCode = 404;
      Serial.println("Sending 404");
    }
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);

  });
}