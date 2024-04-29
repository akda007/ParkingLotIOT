#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "files.h"

const uint8_t size = 3;
const uint8_t sensors[size] = {32, 35, 34};
const uint8_t ledsg[size] = {2, 5, 22};
const uint8_t ledsr[size] = {4, 18, 23};

uint8_t states[size] = {1, 1, 1};

WebServer server(80);

void wifiConnect(const String ssid, const String pw)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);

  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi module connection failed!");
    delay(1000);
    ESP.restart();
  }
}

void getRoot()
{
  server.send(200, "text/html", rootPage);
}

char* storePrintf(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    size_t sz = vsnprintf(NULL, 0, fmt, arg);
    char* buf = (char*)malloc(sz + 1);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    return buf;
}

void getSensorData() {
  char * data = storePrintf("{\"s1\": %1d, \"s2\": %1d, \"s3\": %1d }", states[0], states[1], states[2]);
  server.send(200, "text/json", data);

  free(data);
}

void setupServer()
{
  server.on("/", getRoot);
  server.on("/data", getSensorData);
  server.enableCORS(true);
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void setup()
{
  Serial.begin(115200);

  for (auto i : sensors)
  {
    pinMode(i, INPUT);
  }
  for (auto i : ledsg)
  {
    pinMode(i, OUTPUT);
  }
  for (auto i : ledsr)
  {
    pinMode(i, OUTPUT);
  }

  wifiConnect("A54 de akda", "12345678");
  setupServer();
}

void loop()
{
  server.handleClient();

  delay(2);

  Serial.printf("\rS1: %6d S2: %6d S3: %6d", analogRead(sensors[0]), analogRead(sensors[1]), analogRead(sensors[2]));
  for (int i = 0; i < size; i++)
  {
    if (analogRead(sensors[i]) > 80)
    {
      states[i] = 1;
      digitalWrite(ledsg[i], HIGH);
      digitalWrite(ledsr[i], LOW);

    }
    else
    {
      states[i] = 0;
      digitalWrite(ledsg[i], LOW);
      digitalWrite(ledsr[i], HIGH);
    }
  }
}