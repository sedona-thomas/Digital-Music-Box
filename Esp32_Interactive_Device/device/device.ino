/*
 *
 */

#define BAUDRATE 115200     // baudrate for serial communications
#define DISPLAY_VALUES true // defined: sensors; not defined: rainbow background
#define JSON true           // sends JSON data over serial connection not tagged

#include "esp32_screen.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <list>
#include <stdint.h>
#include <string>

class Button;
class Potentiometer;
class Joystick;

Button button = Button("button1", 37, JSON);
Potentiometer potentiometer = Potentiometer("potentiometer1", 12, JSON);
Joystick joystick = Joystick("joystick1", 27, 26, 25, JSON);

/*
 * Sensor Classes: contols various sensors
 */

class Button {
public:
  std::string name;
  uint8_t pin;
  uint8_t value;
  std::list<uint8_t> values;
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
  for (int i = 0; i < 5; i++) {
    values.push_back(0);
  }
}

Button::Button(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
  json = json_in;
  for (int i = 0; i < 5; i++) {
    values.push_back(0);
  }
}

// read(): reads button value
void Button::read() {
  pinMode(pin, INPUT_PULLUP);
  values.push_back(digitalRead(pin));
  values.pop_front();
  value = (std::find(values.begin(), values.end(), 1) != values.end());
#if DISPLAY_VALUES
  printSensorToScreen("button", value);
#endif
}

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
}

class Potentiometer {
public:
  std::string name;
  uint8_t pin;
  uint8_t value;
  std::list<uint8_t> values;
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
  for (int i = 0; i < 5; i++) {
    values.push_back(0);
  }
}

Potentiometer::Potentiometer(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
  json = json_in;
  for (int i = 0; i < 5; i++) {
    values.push_back(0);
  }
}

// read(): reads potentiometer value
void Potentiometer::read() {
  values.push_back(analogRead(pin));
  values.pop_front();
  int sum = 0;
  for (auto val : values) {
    sum += val;
  }
  value = sum / values.size();
#if DISPLAY_VALUES
  printSensorToScreen("potentiometer" + name, value);
#endif
};

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
#if DISPLAY_VALUES
  printToScreen("joystick");
#endif
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

/*
 * Main Code: runs Esp32 setup and loop
 */

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(BAUDRATE);
  delay(1000);
}

// sendPeripherals(): sends values of all peripherals
void sendPeripherals() {
  if (JSON) {
    Serial.print("{ data:");
    button.send();
    potentiometer.send();
    joystick.send();
    Serial.println("}");
  } else {
    Serial.print("<data>");
    button.send();
    potentiometer.send();
    joystick.send();
    Serial.println("</data>");
  }
}

void setup() {
  setupScreen();
  setupSerial();
}

void loop() {
  updateScreen(DISPLAY_VALUES);
  sendPeripherals();
  delay(FRAMERATE);
}
