#include "SerialCommunication.h"

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