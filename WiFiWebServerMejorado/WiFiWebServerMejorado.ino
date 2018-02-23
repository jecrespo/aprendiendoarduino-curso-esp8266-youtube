/*
  This sketch demonstrates how to set up a simple HTTP-like server.
  Web:
  <!DOCTYPE html>
  <html>
  <body>
  <p>LED APAGADO</p>
  <form action="" method="post">
  <input type="submit" value="Enciende Led" />
  </form>
  </body>
  </html>
  
  server_ip is the IP address of the ESP8266 module, will be
  printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>

const char* ssid = "your-ssid";
const char* password = "your-password";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

boolean led_state = LOW;

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, led_state);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val;
  if (req.indexOf("GET / HTTP/1.1") != -1)
    muestraWeb(client);
  else if (req.indexOf("POST / HTTP/1.1") != -1) {
    Serial.println("------> Cambio estado LED");
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);  //cambio el estado del led
    muestraWeb(client);
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

void muestraWeb(WiFiClient client) {
  client.println("HTTP/1.0 200K");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<body>");
  if (led_state == HIGH) client.println("<p>LED ENCENDIDO</p>");
  else client.println("<p>LED APAGADO</p>");
  client.println("<form action=\"\" method=\"post\">");
  if (led_state == HIGH) client.println("<input type=\"submit\" value=\"Apaga Led\" />");
  else client.println("<input type=\"submit\" value=\"Enciende Led\" />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
  client.println();
  client.stop();
  client.flush();
}

