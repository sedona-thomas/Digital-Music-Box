#ifndef __ESP32_SCREEN_H__
#define __ESP32_SCREEN_H__

#include <SPI.h>
#include <TFT_eSPI.h>
#include <list>
#include <stdint.h>
#include <string>

uint32_t currentBackgroundColor = TFT_WHITE, currentTextColor = TFT_BLACK;
uint8_t currentTextSize = 1; // 10 pixels

void setupScreen(TFT_eSPI tft);
void resetScreen(TFT_eSPI tft);
void updateScreen(TFT_eSPI tft, bool display_values);
std::vector<String> getLetterVector(std::string str);
inline uint16_t randomColor();
inline uint16_t getRGB(uint8_t r, uint8_t g, uint8_t b);
void rainbowBackground(TFT_eSPI tft);

#endif