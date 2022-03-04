/*
 *
 */

class Button {
public:
  std::string name;
  const uint8_t pin;
  bool detected;
  uint8_t value;
  bool json;

  Button(std::string, int, bool);
  Button(int, bool);
  void read();
  void send();
};

Button(std::string _name, int _pin, bool _json) {
  name = _name;
  pin = _pin;
  detected = false;
  value = 0;
  json = _json;
}

Button(int _pin, bool _json) {
  name = "";
  pin = _pin;
  detected = false;
  value = 0;
  json = _json;
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
    if (json) {
      Serial.print("button_" + name + ": ");
      Serial.print(value);
      Serial.print(",");
    } else {
      Serial.print("<button_" + name + ">");
      Serial.print(value);
      Serial.print("</button_" + name + ">");
    }
  }
};

class Potentiometer {
public:
  std::string name;
  const uint8_t pin;
  bool detected;
  uint8_t value;
  bool json;

  Button(std::string, int, bool);
  Button(int, bool);
  void read();
  void send();
};

Potentiometer(std::string _name, int _pin, bool _json) {
  name = _name;
  pin = _pin;
  detected = false;
  value = 0;
  json = _json;
}

Potentiometer(int _pin, bool _json) {
  name = "";
  pin = _pin;
  detected = false;
  value = 0;
  json = _json;
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
    if (json) {
      if (name.length > 0) {
        Serial.print("potentiometer_" + name + ": ");
        Serial.print(value);
        Serial.print(",");
      } else {
        Serial.print("potentiometer: ");
        Serial.print(value);
        Serial.print(",");
      }
    } else {
      if (name.length > 0) {
        Serial.print("<potentiometer_" + name + ">");
        Serial.print(value);
        Serial.print("</potentiometer_" + name + ">");
      } else {
        Serial.print("<potentiometer>");
        Serial.print(value);
        Serial.print("</potentiometer>");
      }
    }
  }
};

class Joystick {
public:
  std::string name;
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;
  bool json;

  Button(std::string, int);
  void read();
  void send();
};

Joystick(std::string _name, int _pinX, int _pinY, int _pinSW, bool _json) {
  name = _name;
  potentiometerX = Potentiometer(_pinX);
  potentiometerY = Potentiometer(_pinY);
  buttonSW = Button(_pinSW);
  json = _json;
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
    Serial.print("joystick_" + name + ": ");
    Serial.print("{");
    Serial.print(value);
    Serial.print("},");
  } else {
    Serial.print("<joystick_" + name + ">");
    potentiometerX.send();
    potentiometerY.send();
    buttonSW.send();
    Serial.print("</joystick_" + name + ">");
  }
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
  bool json = false;
  button = Button("button1", 37, json);
  potentiometer = Potentiometer("potentiometer1", 12, json);
  joystick = Joystick("joystick1", 27, 26, 25, json);
}

// sendPeripherals(): sends values of all peripherals
void setupPeripherals() {
  Serial.print("{ data:");
  button.send();
  potentiometer.send();
  joystick.send();
  Serial.print("}");
}

void setup() { setupSerial(); }
void loop() { sendPeripherals(); }