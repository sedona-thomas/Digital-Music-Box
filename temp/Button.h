#include "Arduino.h"
#include "Sensor.h"
#include "SerialCommunication.h"
#include "ValueQueue.h"
#include "esp32_screen.h"

class Button : public Sensor, public SerialCommunication {
public:
  Button(){};
  Button(std::string, int, bool);
  Button(int, bool);
  void read();
  void send();
};