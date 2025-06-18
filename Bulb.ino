#define LED_PIN D4 // GPIO5 - connected to LED or relay module

String inputString = ""; // String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Start with LED off

  Serial.begin(115200);
  Serial.println("Type ON or OFF to control the LED bulb");
}

void loop() {
  // When a complete string has been received
  if (stringComplete) {
    inputString.trim(); // remove whitespace/newlines

    if (inputString.equalsIgnoreCase("ON")) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED is ON");
    } else if (inputString.equalsIgnoreCase("OFF")) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED is OFF");
    } else {
      Serial.println("Invalid command. Type ON or OFF.");
    }

    // Clear the input
    inputString = "";
    stringComplete = false;
  }
}

// SerialEvent occurs whenever data is received
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}