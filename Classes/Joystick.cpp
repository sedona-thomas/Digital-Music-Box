class Joystick {
public:
  std::string name;
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;

  Button(std::string, int);
  void read();
  void send();
};

Joystick(std::string _name, int _pinX, int _pinY, int _pinSW) {
  name = _name;
  potentiometerX = Potentiometer(_pinX);
  potentiometerY = Potentiometer(_pinY);
  buttonSW = Button(_pinSW);
}

// read(): reads joystick value
void Joystick::read() {
  potentiometerX.read();
  potentiometerY.read();
  buttonSW.read();
};

// send(): sends data from peripheral over the serial connection
void Joystick::send() {
  Serial.print("<joystick_" + name + ">");
  potentiometerX.send();
  potentiometerY.send();
  buttonSW.send();
  Serial.print("</joystick_" + name + ">");
};