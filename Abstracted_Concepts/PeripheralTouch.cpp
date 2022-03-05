class PeripheralTouch {
public:
  std::string name;
  int myNum;
  string myString;
  const uint8_t pin;
  const uint8_t touchPin;
  bool detected;
  uint8_t value;
  void gotTouch();
  void send();
};

PeripheralTouch(std::string _name, int _pin, int _touchPin) {
  name = _name;
  pin = _pin;
  touchPin = _touchPin;
  detected = false;
  value = 0;

  int threshold = 40;
  touchAttachInterrupt(touchPin, gotTouch, threshold);
}

void PeripheralTouch::gotTouch() {
  detected = true;
  value = touchRead(buttonTouchPin);
};

void PeripheralTouch::send() {
  if (detected) {
    detected = false;
    Serial.print("<" + name + ">");
    Serial.print(value);
    Serial.print("</" + name + ">");
  }
};