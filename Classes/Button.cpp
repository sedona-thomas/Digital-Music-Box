class Button {
public:
  std::string name;
  const uint8_t pin;
  bool detected;
  uint8_t value;

  Button(std::string, int);
  void read();
  void send();
};

Button(std::string _name, int _pin) {
  name = _name;
  pin = _pin;
  detected = false;
  value = 0;
}

// read(): reads button value
void Peripheral::read() {
  detected = true;
  pinMode(pin, INPUT_PULLUP);
  digitalRead(pin);
};

// send(): sends data from peripheral over the serial connection
void Button::send() {
  if (detected) {
    detected = false;
    Serial.print("<button_" + name + ">");
    Serial.print(value);
    Serial.print("</button_" + name + ">");
  }
};