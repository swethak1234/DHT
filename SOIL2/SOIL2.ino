const int sensorPin = A0; // Analog pin A0 on NodeMCU
int rawValue = 0;
int moisturePercent = 0;
int maxPercent = 0;
int minPercent = 100;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Soil Moisture Sensor Initialized (Percentage Mode)");
}

void loop() {
  rawValue = analogRead(sensorPin); // Read raw value from sensor

  // Convert raw value to percentage (0% = dry, 100% = wet)
  moisturePercent = map(rawValue, 1023, 0, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100); // Ensure value stays in range

  // Track max and min
  if (moisturePercent > maxPercent) {
    maxPercent = moisturePercent;
  }

  if (moisturePercent < minPercent) {
    minPercent = moisturePercent;
  }

  // Print to Serial Monitor
  Serial.print("Current Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("% | Max: ");
  Serial.print(maxPercent);
  Serial.print("% | Min: ");
  Serial.print(minPercent);
  Serial.println("%");

  delay(2000); // Wait 2 seconds before next read
}