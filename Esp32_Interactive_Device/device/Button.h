#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <list>
#include <stdint.h>
#include <string>

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

#endif