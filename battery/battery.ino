float AnalogWert;
float Powerwert;

void setup() {
  Serial.begin(9600);
}

void loop() {
  AnalogWert = analogRead(A0);
  Powerwert = AnalogWert * (12.0 / 1023); //in case of higher voltage, change the code: powerwert = AnalogWert *(x/1023); x= your new power supply maximum voltage
  Serial.println(Powerwert);
  delay(50);
}
