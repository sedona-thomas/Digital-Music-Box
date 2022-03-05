class Button {
public:
  std::string name;
  uint8_t pin;
  uint8_t value;
  bool json;

  Button() {}
  Button(std::string, int, bool);
  Button(int, bool);
  void read();
  void send();
};

Button::Button(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  value = 0;
  json = json_in;
}

Button::Button(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
  json = json_in;
}

// read(): reads button value
void Button::read() {
  pinMode(pin, INPUT_PULLUP);
  value = digitalRead(pin);
  tft.println("button");
  tft.println(value);
};

// send(): sends data from peripheral over the serial connection
void Button::send() {
  Button::read();
  if (json) {
    Serial.print("button_");
    Serial.print(name.c_str());
    Serial.print(": ");
    Serial.print(value);
    Serial.print(",");
  } else {
    Serial.print("<button_");
    Serial.print(name.c_str());
    Serial.print(">");
    Serial.print(value);
    Serial.print("</button_");
    Serial.print(name.c_str());
    Serial.print(">");
  }
};