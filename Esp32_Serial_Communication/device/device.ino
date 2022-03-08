/*
 * device.ino: reads values from an Esp32 and sends them as serial data
 */

#define BAUDRATE 115200     // baudrate for serial communications
#define DISPLAY_VALUES true // true: sensors; false: rainbow background
#define JSON true           // sends JSON data over serial connection not tagged
#define BUTTON_DELAY 50     // delays button if defined

#include "ValueQueue.h"
#include "esp32_screen.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <list>
#include <stdint.h>
#include <string>

class Sensor {
protected:
  uint8_t pin;
  uint8_t value;
  ValueQueue values;
  virtual void read() { value = analogRead(pin); };
};

class SerialCommunication {
protected:
  std::string name;
  bool json;
  void send(){};
  void sendSerialObject(std::string, uint8_t);
};

class Button : public Sensor, public SerialCommunication {
public:
  Button(){};
  Button(std::string, int, bool);
  Button(int, bool);
  void read();
  void send();
};

class Potentiometer : public Sensor, public SerialCommunication {
public:
  Potentiometer(){};
  Potentiometer(std::string, int, bool);
  Potentiometer(int, bool);
  void read();
  void send();
};

class Joystick : public SerialCommunication {
public:
  Joystick(){};
  Joystick(std::string, int, int, int, bool);
  void send();
  void sendSerialObject();

private:
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;
};

////////////////////////////////////////////////////////////////////////////////

Button button = Button("mode", 37, JSON);
Potentiometer potentiometer = Potentiometer("octave", 12, JSON);
Joystick joystick = Joystick("joystick1", 27, 26, 25, JSON);

void setupSerial();
void sendPeripherals();

void setup() {
  setupScreen();
  setupSerial();
}

void loop() {
  updateScreen(DISPLAY_VALUES);
  sendPeripherals();
  delay(FRAMERATE);
}

////////////////////////////////////////////////////////////////////////////////

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(BAUDRATE);
  delay(1000);
}

// sendPeripherals(): sends values of all peripherals
void sendPeripherals() {
  if (JSON) {
    Serial.print("{ \"data\": {");
    button.send();
    Serial.print(",");
    potentiometer.send();
    Serial.print(",");
    joystick.send();
    Serial.println("}}");
  } else {
    Serial.print("<data>");
    button.send();
    potentiometer.send();
    joystick.send();
    Serial.println("</data>");
  }
}

////////////////////////////////////////////////////////////////////////////////

Button::Button(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  value = 0;
#ifdef BUTTON_DELAY
  values = ValueQueue(BUTTON_DELAY);
#endif
  json = json_in;
}

Button::Button(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
#ifdef BUTTON_DELAY
  values = ValueQueue(BUTTON_DELAY);
#endif
  json = json_in;
}

// read(): reads button value
void Button::read() {
  pinMode(pin, INPUT_PULLUP);
  values.add(digitalRead(pin));
  value = values.contains(1) ? 1 : 0;
#if DISPLAY_VALUES
  printSensorToScreen("button", value);
#endif
}

// send(): sends data from peripheral over the serial connection
void Button::send() {
  read();
  sendSerialObject("button", value);
}

////////////////////////////////////////////////////////////////////////////////

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
void Potentiometer::read() {
  values.add(analogRead(pin));
  value = values.median();
#if DISPLAY_VALUES
  printSensorToScreen("potentiometer" + name, value);
#endif
};

// send(): sends data from peripheral over the serial connection
void Potentiometer::send() {
  read();
  sendSerialObject("potentiometer", value);
};

////////////////////////////////////////////////////////////////////////////////

Joystick::Joystick(std::string name_in, int pin_X, int pin_Y, int pin_SW,
                   bool json_in) {
  name = name_in;
  potentiometerX = Potentiometer("x", pin_X, json_in);
  potentiometerY = Potentiometer("y", pin_Y, json_in);
  buttonSW = Button("sw", pin_SW, json_in);
  json = json_in;
}

// send(): sends data from peripheral over the serial connection
void Joystick::send() {
#if DISPLAY_VALUES
  printToScreen("joystick");
#endif
  sendSerialObject();
};

////////////////////////////////////////////////////////////////////////////////

// send(): sends data from peripheral over the serial connection
void SerialCommunication::sendSerialObject(std::string sensor, uint8_t value) {
  if (json) {
    if (name.length() > 0) {
      Serial.print("\"");
      Serial.print(sensor.c_str());
      Serial.print("_");
      Serial.print(name.c_str());
      Serial.print("\": ");
    } else {
      Serial.print("\"");
      Serial.print(sensor.c_str());
      Serial.print("\": ");
    }
    Serial.print(value);
  } else {
    if (name.length() > 0) {
      Serial.print("<");
      Serial.print(sensor.c_str());
      Serial.print("_");
      Serial.print(name.c_str());
      Serial.print(">");
      Serial.print(value);
      Serial.print("</");
      Serial.print(sensor.c_str());
      Serial.print("_");
      Serial.print(name.c_str());
      Serial.print(">");
    } else {
      Serial.print("<");
      Serial.print(sensor.c_str());
      Serial.print(">");
      Serial.print(value);
      Serial.print("</");
      Serial.print(sensor.c_str());
      Serial.print(">");
    }
  }
}

void Joystick::sendSerialObject() {
  if (json) {
    if (name.length() > 0) {
      Serial.print("\"joystick_");
      Serial.print(name.c_str());
      Serial.print("\": ");
    } else {
      Serial.print("\"joystick\": ");
    }
    Serial.print("{");
    potentiometerX.send();
    Serial.print(",");
    potentiometerY.send();
    Serial.print(",");
    buttonSW.send();
    Serial.print("}");
  } else {
    Serial.print("<joystick_");
    Serial.print(name.c_str());
    Serial.print(">");
    potentiometerX.send();
    Serial.print(",");
    potentiometerY.send();
    Serial.print(",");
    buttonSW.send();
    Serial.print("</joystick_");
    Serial.print(name.c_str());
    Serial.print(">");
  }
}