class Potentiometer {
public:
  std::string name;
  const uint8_t pin;
  bool detected;
  uint8_t value;

  Button(std::string, int);
  void read();
  void send();
};

Potentiometer(std::string _name, int _pin) {
  name = _name;
  pin = _pin;
  detected = false;
  value = 0;
}

// read(): reads potentiometer value
void Potentiometer::read() {
  detected = true;
  analogRead(pin);
};

// send(): sends data from peripheral over the serial connection
void Potentiometer::send() {
  if (detected) {
    detected = false;
    Serial.print("<potentiometer_" + name + ">");
    Serial.print(value);
    Serial.print("</potentiometer_" + name + ">");
  }
};