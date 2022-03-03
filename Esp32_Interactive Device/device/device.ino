/*
 *
 */

class Button {
public:
  const uint8_t pin;
  bool detected;
  uint8_t value;

  Button(std::string, int);
  void read();
  void send();
};

Button(int _pin) {
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
    Serial.print("<button>");
    Serial.print(value);
    Serial.print("</button>");
  }
};

class Potentiometer {
public:
  const uint8_t pin;
  bool detected;
  uint8_t value;

  Button(std::string, int);
  void read();
  void send();
};

Potentiometer(int _pin) {
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
    Serial.print("<potentiometer>");
    Serial.print(value);
    Serial.print("</potentiometer>");
  }
};

class Joystick {
public:
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;

  Button(std::string, int);
  void read();
  void send();
};

Joystick(std::string _name, int _pinX, int _pinY, int _pinSW) {
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
  Serial.print("<joystick>");
  potentiometerX.send();
  potentiometerY.send();
  buttonSW.send();
  Serial.print("</joystick>");
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
  button = Button(37);
  potentiometer = Potentiometer(12);
  joystick = Joystick(27, 26, 25);
}

// sendPeripherals(): sends values of all peripherals
void setupPeripherals() {
  button.send();
  potentiometer.send();
  joystick.send();
}

void setup() { setupSerial(); }
void loop() { sendPeripherals(); }