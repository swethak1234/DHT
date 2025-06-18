#include <ESP8266WiFi.h>

// WiFi credentials
const char* ssid = "OPPO F23 5G";
const char* password = "88888888";

// Built-in LED pin (D4 / GPIO2)
const int ledPin = LED_BUILTIN;
bool ledOn = false; // Track LED state (false = OFF)

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Start with LED OFF (inverted logic)

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
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
          // Handle toggle request
          if (request.indexOf("GET /TOGGLE") >= 0) {
            ledOn = !ledOn; // Toggle state
            digitalWrite(ledPin, ledOn ? LOW : HIGH); // Inverted logic
          }

          // Send webpage response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          String ledStatus = ledOn ? "ON" : "OFF";
          String buttonLabel = ledOn ? "Turn OFF" : "Turn ON";

          client.println("<!DOCTYPE html><html><head><title>LED Toggle</title></head><body style='text-align:center;'>");
          client.println("<h1>NodeMCU LED Control</h1>");
          client.println("<p>LED is currently: <strong>" + ledStatus + "</strong></p>");
          client.println("<a href=\"/TOGGLE\"><button style='padding:10px 20px;'>" + buttonLabel + "</button></a>");
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