#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_PIN A0
#define PUMP_PIN 4

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial esp8266(10, 11); // RX, TX (Arduino pins to ESP8266 TX/RX)

// WiFi Credentials
String ssid = "OPPO F23 5G";
String password = "88888888";

// ThingSpeak API
String apiKey = "TLKD3KXIL1CFDC8M";

void setup() {
  Serial.begin(9600);
  esp8266.begin(9600);
  dht.begin();

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); // Initially OFF

  connectWiFi();
}

void loop() {
  float temp = dht.readTemperature();
  int soilRaw = analogRead(SOIL_PIN);
  int soilPercent = map(soilRaw, 1023, 0, 0, 100);
  bool pumpOn = false;

  if (soilPercent < 40 || temp > 36) {
    digitalWrite(PUMP_PIN, HIGH);
    pumpOn = true;
    Serial.println("Pump ON");
  } else {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Pump OFF");
  }

  sendDataToThingSpeak(temp, soilPercent, pumpOn);
  delay(20000); // ThingSpeak requires 15+ seconds between requests
}

// Function to connect ESP8266 to WiFi
void connectWiFi() {
  esp8266.println("AT");
  delay(2000);
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  esp8266.println(cmd);
  delay(6000);
  Serial.println("WiFi Connected");
}

// Function to send data to ThingSpeak
void sendDataToThingSpeak(float temp, int soilMoisture, bool pumpStatus) {
  String data = "GET /update?api_key=" + apiKey;
  data += "&field1=" + String(temp);
  data += "&field2=" + String(soilMoisture);
  data += "&field3=" + String(pumpStatus ? 1 : 0);
  data += "\r\n";

  esp8266.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
  delay(2000);

  String sendCmd = "AT+CIPSEND=";
  sendCmd += String(data.length() + 2); // +2 for \r\n
  esp8266.println(sendCmd);
  delay(2000);

  esp8266.println(data);
  delay(3000);
  esp8266.println("AT+CIPCLOSE");
}