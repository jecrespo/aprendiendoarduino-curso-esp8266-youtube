/*
    This sketch sends a message to www.aprendiendoarduino.com server to discover secret message

*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "password");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  const uint16_t port = 80;
  const char * host = "www.aprendiendoarduino.com"; // ip or dns

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  // This will send the request to the server
  client.println("GET /servicios/aprendiendoarduino/ HTTP/1.0");
  client.println("Host: www.aprendiendoarduino.com");
  client.println();

  while (client.available() == 0) {
    static int count = 0;
    Serial.print(".");
    delay(250);
    if (count > 12) //waiting more than 3 seconds
      break;
  }
  Serial.println();

  if (client.connected()) {
    //read all lines from server
    do {
      String line = client.readStringUntil('\r'); //read line by line
      Serial.print(line);
    } while (client.available() > 0);
    Serial.println();
  }
  else {
    Serial.println("Connection error.");
  }

  Serial.println("closing connection");
  client.stop();

  Serial.println("wait 5 sec...");
  delay(5000);
}
