#include <ESP8266WiFi.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "OPPO F23 5G";
const char* password = "88888888";

// DHT settings
#define DHTPIN D4      // DHT11 data pin connected to GPIO2 (D4)
#define DHTTYPE DHT11  // Define sensor type
DHT dht(DHTPIN, DHTTYPE);

// Create a web server on port 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          // Read temperature
          float temperature = dht.readTemperature();
          String tempString = isnan(temperature) ? "Error reading temperature!" : String(temperature, 1) + " °C";

          // Send response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><head><title>DHT11 Temp</title></head><body>");
          client.println("<h1>Temperature Reading</h1>");
          client.println("<p>Current Temperature: <strong>" + tempString + "</strong></p>");
          client.println("</body></html>");
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client Disconnected.");
  }
}