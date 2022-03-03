/*
 *
 */

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

Button button;
Potentiometer potentiometer;
Joystick joystick;

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000);
  setupPeripherals();
}

// setupPeripherals(): sets up connections to all peripherals
void setupPeripherals() {
  button = Button("button1", 37);
  potentiometer = Potentiometer("potentiometer1", 12);
  joystick = Joystick("joystick1", 27, 26, 25);
}

// sendPeripherals(): sends values of all peripherals
void setupPeripherals() {
  button.send();
  potentiometer.send();
  joystick.send();
}

void setup() { setupSerial(); }
void loop() { sendPeripherals(); }