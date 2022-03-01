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

int threshold = 40;

// main
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
