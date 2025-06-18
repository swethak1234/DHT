#include <ESP8266WiFi.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "OPPO F23 5G";
const char* password = "88888888";

// DHT sensor setup
#define DHTPIN D4        // DHT11 data pin connected to GPIO2 (D4)
#define DHTTYPE DHT11    // Sensor type
DHT dht(DHTPIN, DHTTYPE);

// Web server runs on port 80
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
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected.");
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        // End of HTTP request
        if (c == '\n') {
          // Read sensor data
          float temp = dht.readTemperature();
          float hum = dht.readHumidity();

          // Prepare strings
          String tempStr = isnan(temp) ? "Error" : String(temp, 1) + " °C";
          String humStr = isnan(hum) ? "Error" : String(hum, 1) + " %";

          // Send HTTP response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><head><title>DHT11 Sensor</title></head><body>");
          client.println("<h1>DHT11 Sensor Readings</h1>");
          client.println("<p><strong>Temperature:</strong> " + tempStr + "</p>");
          client.println("<p><strong>Humidity:</strong> " + humStr + "</p>");
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