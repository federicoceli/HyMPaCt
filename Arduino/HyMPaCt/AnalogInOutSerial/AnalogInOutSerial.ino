const int analogInPin  = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 3;

double res = 1024.0;
double volt = 5.0;

int sensorValue = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(analogOutPin, OUTPUT);
  //pinMode(analogInPin, INPUT);
  //analogWrite(analogOutPin, 51);
}

void loop() {
  // analogReadResolution(12);
  // read the analog in value:
  analogWrite(analogOutPin, 103);

  delay(100);

  sensorValue = analogRead(analogInPin);

  Serial.print("ADC = ");
  Serial.print(sensorValue);
  
  Serial.print("\tVolt = ");
  Serial.print(sensorValue/res*volt);
  
  Serial.print("\tflux = ");
  Serial.println(sensorValue/res*volt/1.6);

  delay(100);
}
