const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 3;

int res = 1024.0;
int volt = 5;

int sensorValue = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(analogOutPin, OUTPUT);
  analogWrite(analogOutPin, 300);
}

void loop() {
 // analogReadResolution(12);
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  Serial.print("ADC = ");
  Serial.print(sensorValue);
  
  Serial.print("\tVolt = ");
  Serial.print(sensorValue/res*volt);
  
  Serial.print("\tflux = ");
  Serial.println(sensorValue/res*volt/1.6);

  delay(200);
}
