const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  analogReadResolution(12);
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  Serial.print("ADC = ");
  Serial.print(sensorValue);
  
  Serial.print("\tVolt = ");
  Serial.print(sensorValue/4095.0*3.3);
  
  Serial.print("\tflux = ");
  Serial.println(sensorValue/4095.0*3.3*80/1.6);

  delay(200);
}
