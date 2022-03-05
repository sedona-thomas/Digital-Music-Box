class Potentiometer {
public:
  std::string name;
  uint8_t pin;
  uint8_t value;
  bool json;

  Potentiometer() {}
  Potentiometer(std::string, int, bool);
  Potentiometer(int, bool);
  void read();
  void send();
};

Potentiometer::Potentiometer(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  value = 0;
  json = json_in;
}

Potentiometer::Potentiometer(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
  json = json_in;
}

// read(): reads potentiometer value
void Potentiometer::read() { value = analogRead(pin); };

// send(): sends data from peripheral over the serial connection
void Potentiometer::send() {
  Potentiometer::read();
  if (json) {
    if (name.length() > 0) {
      Serial.print("potentiometer_");
      Serial.print(name.c_str());
      Serial.print(": ");
      Serial.print(value);
      Serial.print(",");
    } else {
      Serial.print("potentiometer: ");
      Serial.print(value);
      Serial.print(",");
    }
  } else {
    if (name.length() > 0) {
      Serial.print("<potentiometer_");
      Serial.print(name.c_str());
      Serial.print(">");
      Serial.print(value);
      Serial.print("</potentiometer_");
      Serial.print(name.c_str());
      Serial.print(">");
    } else {
      Serial.print("<potentiometer>");
      Serial.print(value);
      Serial.print("</potentiometer>");
    }
  }
};