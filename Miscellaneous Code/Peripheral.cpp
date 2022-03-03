class Peripheral {
public:
  std::string name;
  const uint8_t pin;
  bool detected;
  uint8_t value;

  Peripheral(std::string, int);
  void read();
  void send();
};

Peripheral(std::string _name, int _pin) {
  name = _name;
  pin = _pin;
  detected = false;
  value = 0;
}

// read(): reads peripheral value
void Peripheral::read() {
  detected = true;
  pinMode(pin, INPUT_PULLUP);
  digitalRead(pin);
};

// send(): sends data from peripheral over the serial connection
void Peripheral::send() {
  if (detected) {
    detected = false;
    Serial.print("<" + name + ">");
    Serial.print(value);
    Serial.print("</" + name + ">");
  }
};