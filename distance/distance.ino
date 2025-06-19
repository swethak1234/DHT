#define ECHOPIN D5
#define TRIGPIN D6
void setup() {
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
    Serial.begin(9600);
    Serial.println("Ultrasonic Sensor Test");
    digitalWrite(TRIGPIN, LOW);

}   
void loop() {
  float distance = measureDistance(TRIGPIN,ECHOPIN);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

float measureDistance(int TRIG,int ECHO)
{
digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
  float duration = pulseIn(ECHOPIN, HIGH);
 float distance = (duration * 0.034) / 2; // Convert to cm
 return distance;
}
