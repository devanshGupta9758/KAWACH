#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#define mq3_pin 26
#define ir_pin 25

float sensor_value;

const char *ssid = "helmet";
const char *password = "123456789";

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);




IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

String readIR(int ir, float sVal) {
  return (String(ir) + "  "+ String(sVal));
}

String readmq3(int mq3, float sVal) {
  return (String(mq3) + "  "+ String(sVal));
}



void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  pinMode(mq3_pin, INPUT);
  pinMode(ir_pin, INPUT);
  server.begin();
  Serial.println("MQ3 is warming up");
  delay(2000);  //2 min warm up time
}

void loop() {
  //int i =
  sensor_value = digitalRead(ir_pin); 
  server.on("/ir", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readIR(digitalRead(ir_pin), sensor_value).c_str());
  });
  int mq3_sensor_value = digitalRead(mq3_pin); 
  server.on("/mq3", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readmq3(digitalRead(mq3_pin), sensor_value).c_str());
  });
  

  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());
  delay(500);
  Serial.print("IR Value: ");
  Serial.println(sensor_value);
  Serial.print("MQ3 Value: ");
  Serial.println(mq3_sensor_value);
  delay(2000);
  }
