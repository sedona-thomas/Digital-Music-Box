#include "helper.h"

// getLetterVector(): turns a std::string into an Arduino String vector
std::vector<String> getLetterVector(std::string str) {
  std::vector<String> letters;
  for (int i = 0; i < str.size(); i++) {
    letters.push_back(String(str[i]));
  }
  return letters;
}

// randomColor(): outputs a random pastel RGB color
inline uint16_t randomColor() {
  return getRGB(random(0, 255) / 2, random(0, 255) / 2, random(0, 255) / 2);
}

// getRGB(): converts an RGB value to an Arduino color value
// https://stackoverflow.com/questions/13720937/c-defined-16bit-high-color
inline uint16_t getRGB(uint8_t r, uint8_t g, uint8_t b) {
  return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

// rainbowBackground(): makes the background a scrolling rainbow gradient
void rainbowBackground(TFT_eSPI tft) {
  byte red = 31, green = 0, blue = 0, state = 0;
  unsigned int colour = red << 11;
  for (int i = 0; i < tft.width(); i++) {
    tft.drawFastVLine(i, 0, tft.height(), colour);
    switch (state) {
    case 0:
      green += 2;
      if (green == 64) {
        green = 63;
        state = 1;
      }
      break;
    case 1:
      red--;
      if (red == 255) {
        red = 0;
        state = 2;
      }
      break;
    case 2:
      blue++;
      if (blue == 32) {
        blue = 31;
        state = 3;
      }
      break;
    case 3:
      green -= 2;
      if (green == 255) {
        green = 0;
        state = 4;
      }
      break;
    case 4:
      red++;
      if (red == 32) {
        red = 31;
        state = 5;
      }
      break;
    case 5:
      blue--;
      if (blue == 255) {
        blue = 0;
        state = 0;
      }
      break;
    }
    colour = red << 11 | green << 5 | blue;
  }
}