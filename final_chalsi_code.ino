#include <WiFi.h>
#include <HTTPClient.h>

const char* svrir = "http://192.168.4.1:80/ir";
const char* svrmq3 = "http://192.168.4.1:80/mq3";

int MOTOR = 5; // conect Led to arduino pin 13
const int Buzz = 18;

void setup()
{
  Serial.begin(115200);
  delay(800);

  WiFi.begin("helmet", "123456789");
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  pinMode (MOTOR, OUTPUT); // Led pin OUTPUT
  pinMode (Buzz, OUTPUT); // Led pin OUTPUT
}


void loop()
{
  if (WiFi.status() == WL_CONNECTED ) {
    String ir_value = httpGETRequest(svrir);
    String mq3_value = httpGETRequest(svrmq3);

    String irString = ir_value.substring(0, ir_value.indexOf(" "));
    String mq3String = mq3_value.substring(0, mq3_value.indexOf(" "));

    Serial.print("IR: ");
    Serial.print(ir_value);
    Serial.print("MQ3: ");
    Serial.println(mq3_value);


    int ir = irString.toInt();
    int mq3 = mq3String.toInt();


    if ((ir == 1 && mq3 == 1) || (ir == 0 && mq3 == 0))
    {
      Serial.println("Gaddi ruk jayegi");
      digitalWrite(MOTOR, HIGH); //  OFF
      digitalWrite(Buzz, HIGH); // ON
    }
    else if (ir == 0 && mq3 == 1)
    {
      Serial.println("Gaddi chalegi");
      digitalWrite(MOTOR, LOW); //  ON
      digitalWrite(Buzz, LOW); // OFF
    }
    ir = 1;
    mq3 = 1;
    //}

  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(800);
}


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
