#include "Arduino.h"
#include "ValueQueue.h"

class SerialCommunication {
protected:
  std::string name;
  bool json;
  void send(){};
  void sendSerialObject(std::string, uint8_t);
};
