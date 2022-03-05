#ifndef __ESP32_SCREEN_H__
#define __ESP32_SCREEN_H__

#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdint.h>
#include <string>

#define WAIT 500     // miliseconds
#define FRAMERATE 50 // miliseconds

void setupScreen();
void resetScreen();
void updateScreen(bool display_values);
void printToScreen(std::string s);
void printSensorToScreen(std::string name, uint8_t value);
String getArduinoString(std::string str);
std::vector<String> getLetterVector(std::string str);
inline uint16_t randomColor();
inline uint16_t getRGB(uint8_t r, uint8_t g, uint8_t b);
void rainbowBackground();

#endif