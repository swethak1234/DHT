#include <ESP8266WiFi.h>

// Set your access point credentials
const char* ssid = "NodeMCU_AP";
const char* password = "12345678"; // At least 8 characters

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Create WiFi access point
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP(); // Get IP address of AP
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected.");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n') {
          // Send HTTP response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>");
          client.println();
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client Disconnected.");
  }
}