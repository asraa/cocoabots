void setup() {
delay(1000);
Serial.begin(9600);
}

void loop()
{
  Serial.println(digitalRead(7),BIN); 
  delay(100);

}




