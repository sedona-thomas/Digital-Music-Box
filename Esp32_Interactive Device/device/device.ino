/*
 *
 */

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

//
//
//

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

//
//
//
//
//
//
//

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

/*
 * main code
 */

int threshold = 40;

void setupSerial();
void setupPeripherals();
int readPin(int pin);

void setup() { setupSerial(); }

void loop() { sendPeripherals(); }

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000);
  setupPeripherals();
}

// setupPeripherals(): sets up connections to all peripherals
void setupPeripherals() {
  Peripheral button = Peripheral("button", 12);
  Peripheral potentiometer = Peripheral("potentiometer", 13);
  Peripheral joystickVRx = Peripheral("joystickVRx", 27);
  Peripheral joystickVRy = Peripheral("joystickVRy", 33);
  Peripheral joystickSW = Peripheral("joystickSW", 32);
}

// readPin(): reads in the value of the specified pin
int readPin(int pin) { return analogRead(pin); }
