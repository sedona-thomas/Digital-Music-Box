#ifndef __HELPER_H__
#define __HELPER_H__

#include <SPI.h>
#include <TFT_eSPI.h>
#include <list>
#include <stdint.h>
#include <string>

std::vector<String> getLetterVector(std::string str);
inline uint16_t randomColor();
inline uint16_t getRGB(uint8_t r, uint8_t g, uint8_t b);
void rainbowBackground(TFT_eSPI tft);

#endif