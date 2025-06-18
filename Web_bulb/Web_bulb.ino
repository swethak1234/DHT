#include <ESP8266WiFi.h>

// WiFi credentials
const char* ssid = "OPPO F23 5G";
const char* password = "88888888";

// LED pin (NodeMCU built-in LED is usually on GPIO2 / D4)
const int ledPin = LED_BUILTIN;
String ledState = "OFF"; // Track the LED state

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LED OFF (inverted logic)

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected");
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n') {
          // Process HTTP request
          if (request.indexOf("GET /ON") >= 0) {
            digitalWrite(ledPin, LOW); // Turn ON (inverted)
            ledState = "ON";
          } else if (request.indexOf("GET /OFF") >= 0) {
            digitalWrite(ledPin, HIGH); // Turn OFF
            ledState = "OFF";
          }

          // Send web page response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><head><title>LED Control</title></head><body style='text-align:center;'>");
          client.println("<h1>Control NodeMCU LED</h1>");
          client.println("<p>LED is currently: <strong>" + ledState + "</strong></p>");
          client.println("<a href=\"/ON\"><button style='padding:10px 20px; margin:10px;'>Turn ON</button></a>");
          client.println("<a href=\"/OFF\"><button style='padding:10px 20px; margin:10px;'>Turn OFF</button></a>");
          client.println("</body></html>");
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client Disconnected");
  }
}