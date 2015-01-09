const int echoPin = 3;
const int trigPin = 2;

const int numReadings = 10;

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  readings[index] = pulseIn(echoPin, HIGH);        // Read in times pulse
  total = total + readings[index];
  index = index + 1;
  if (index >= numReadings) {
     index = 0;
     average = total / numReadings;
     int distance = (average / 74) / 2;
                       // Calculate distance from time of pulse
     Serial.println(distance); 
     total = 0;    }                
  delay(100);                                    // Wait 50mS before next ranging
}

