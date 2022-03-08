#include "Arduino.h"
#include "ValueQueue.h"

class Sensor {
protected:
  uint8_t pin;
  uint8_t value;
  ValueQueue values;
  virtual void read() { value = analogRead(pin); };
};