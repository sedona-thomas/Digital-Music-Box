#include "Button.h"

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