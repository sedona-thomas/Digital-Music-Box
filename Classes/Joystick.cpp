class Joystick {
public:
  std::string name;
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;
  bool json;

  Joystick() {}
  Joystick(std::string, int, int, int, bool);
  void read();
  void send();
};

Joystick::Joystick(std::string name_in, int pin_X, int pin_Y, int pin_SW,
                   bool json_in) {
  name = name_in;
  potentiometerX = Potentiometer("_x" + name_in, pin_X, json_in);
  potentiometerY = Potentiometer("_y" + name_in, pin_Y, json_in);
  buttonSW = Button("_sw" + name_in, pin_SW, json_in);
  json = json_in;
}

// read(): reads joystick value
void Joystick::read() {
  potentiometerX.read();
  potentiometerY.read();
  buttonSW.read();
};

// send(): sends data from peripheral over the serial connection
void Joystick::send() {
  if (json) {
    Serial.print("joystick_");
    Serial.print(name.c_str());
    Serial.print(": ");
    Serial.print("{");
    potentiometerX.send();
    potentiometerY.send();
    buttonSW.send();
    Serial.print("},");
  } else {
    Serial.print("<joystick_");
    Serial.print(name.c_str());
    Serial.print(">");
    potentiometerX.send();
    potentiometerY.send();
    buttonSW.send();
    Serial.print("</joystick_");
    Serial.print(name.c_str());
    Serial.print(">");
  }
};