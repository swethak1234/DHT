const int irSensorPin = 2;   // IR sensor connected to digital pin 2
const int ledPin = 13;       // LED connected to digital pin 13
bool ledState = false;       // Stores the current LED state (ON/OFF)
bool lastSensorState = LOW;  // To detect state change
bool sensorTriggered = false;

void setup() {
  pinMode(irSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(irSensorPin);

  // Check for a transition from LOW to HIGH (hand placed)
  if (sensorValue == HIGH && lastSensorState == LOW && !sensorTriggered) {
    ledState = !ledState;  // Toggle LED state
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    sensorTriggered = true;  // Prevent repeated toggling while hand is in place
    Serial.println(ledState ? "LED ON" : "LED OFF");
  }

  // Reset trigger when hand is removed (transition to LOW)
  if (sensorValue == LOW) {
    sensorTriggered = false;
  }

  lastSensorState = sensorValue;
}