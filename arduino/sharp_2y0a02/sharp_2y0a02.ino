const int IRpin = 1;
const int numReadings = 10;

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

void setup() {
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

void loop() {
  readings[index] = analogRead(IRpin);
  total = total + readings[index];
  index = index + 1;
  
  if (index >= numReadings) {
    index = 0;
    average = total / numReadings;
    float volts = average*0.0048828125;
    float distcm = 65*pow(volts,-1.10);
    float distin = distcm/2.54;
    if (volts < 0.2) distin = -1.0;
    Serial.println(distin);
    Serial.println(total);
    total = 0;
  }
  delay(50);
}



