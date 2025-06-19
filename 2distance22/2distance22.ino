// Dual Ultrasonic Sensor Distance Measurement with Threshold
// NodeMCU Example: Left and Right Distance Sensing and Decision Output

#define TRIG_LEFT  D6
#define ECHO_LEFT  D5

#define TRIG_RIGHT D8
#define ECHO_RIGHT D7

// Set threshold in centimeters
const long threshold = 10; // Change this to your preferred threshold value

long getDistance(int trigPin, int echoPin) {
  long duration;
  long distance;

  // Ensure trigger pin is LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10us HIGH pulse to trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse duration (timeout 25ms = ~4m distance)
  duration = pulseIn(echoPin, HIGH, 25000);

  // If no echo received (timeout), set distance as -1
  if (duration == 0) {
    distance = -1;
  } else {
    distance = duration * 0.034 / 2;  // Convert to cm
  }
  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);

  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Set trigger pins LOW initially
  digitalWrite(TRIG_LEFT, LOW);
  digitalWrite(TRIG_RIGHT, LOW);

  Serial.println("Ultrasonic Sensor Distance Measurement Start");
  Serial.print("Threshold set to: ");
  Serial.print(threshold);
  Serial.println(" cm");
  Serial.println("Left(cm)\tRight(cm)\tDirection");
}

void loop() {
  long leftDistance = getDistance(TRIG_LEFT, ECHO_LEFT);
  long rightDistance = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  Serial.print(leftDistance == -1 ? "Timeout" : String(leftDistance));
  Serial.print("\t\t");
  Serial.print(rightDistance == -1 ? "Timeout" : String(rightDistance));
  Serial.print("\t\t");

  // Only act if both sensors detect an object (i.e., not timeout)
  if (leftDistance == -1 && rightDistance == -1) {
    Serial.println("No object detected");
  } else if (leftDistance == -1) {
    Serial.println("Object only on RIGHT");
  } else if (rightDistance == -1) {
    Serial.println("Object only on LEFT");
  } else {
    long diff = leftDistance - rightDistance;
    if (abs(diff) < threshold) {
      Serial.println("Both sides within threshold. Stay centered.");
    } else if (diff < 0) { // left is closer
      Serial.println("Move toward LEFT (closer object detected)");
    } else { // right is closer
      Serial.println("Move toward RIGHT (closer object detected)");
    }
  }

  delay(1000);
}