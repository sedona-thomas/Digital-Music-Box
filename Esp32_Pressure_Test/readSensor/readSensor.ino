const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the sensor
int outputValue = 0;        // value output to the serial line

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  //Serial.println(sensorValue);
  //you might need to adjust the range depending on your sensor
  outputValue = map(sensorValue, 335, 355, 0, 255);

  // print the results to the Serial Monitor:
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after ○the last reading:
  delay(2);
}