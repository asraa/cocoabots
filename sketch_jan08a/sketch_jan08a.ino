const int echoPin = 3;
const int trigPin = 2;

void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int distance = pulseIn(echoPin, HIGH);        // Read in times pulse
  distance= distance/58;                        // Calculate distance from time of pulse
  Serial.println(distance);                     
  delay(100);                                    // Wait 50mS before next ranging
}

