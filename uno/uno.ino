#include <DHT.h>

// ----- Pin Definitions -----
#define DHTPIN 2            // DHT sensor connected to digital pin 2
#define DHTTYPE DHT11       // Or DHT22
#define SOIL_PIN A0         // Soil moisture sensor connected to analog pin A0
#define PUMP_PIN 4         // Relay control connected to digital pin 8

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);             // Serial Monitor
  dht.begin();                    // Initialize DHT sensor

  pinMode(PUMP_PIN, OUTPUT);      // Set pump relay pin as output
  digitalWrite(PUMP_PIN, LOW);    // Make sure pump is off at start

  Serial.println("Arduino UNO - Pump Control System Initialized");
}

void loop() {
  // Read temperature
  float temperature = dht.readTemperature(); // in Celsius

  // Read soil moisture sensor
  int rawSoil = analogRead(SOIL_PIN);               // Raw reading (0-1023)
  int soilMoisturePercent = map(rawSoil, 474, 243, 0, 100); // % conversion

  //Display sensor data
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.print("% | Temperature: ");
   Serial.print(temperature);
  Serial.println("°C");
  // Serial.println(rawSoil);

  // Trigger pump based on conditions
  if (soilMoisturePercent < 40 || temperature > 36) {
    digitalWrite(PUMP_PIN, HIGH);  // Turn pump ON
    Serial.println("Pump Status: ON");
  } else {
    digitalWrite(PUMP_PIN, LOW);   // Turn pump OFF
    Serial.println("Pump Status: OFF");
  }

  delay(2000); // Wait 2 seconds before next check
}