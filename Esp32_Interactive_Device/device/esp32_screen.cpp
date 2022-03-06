#include "esp32_screen.h"

TFT_eSPI tft = TFT_eSPI();
uint32_t currentBackgroundColor = TFT_WHITE, currentTextColor = TFT_BLACK;
uint8_t currentTextSize = 1; // 10 pixels

// setupScreen(): starts ESP32 screen
void setupScreen() {
  tft.init();
  tft.setRotation(2);
}

// resetScreen(): resets the background and text color/size of the display
void resetScreen() {
  tft.setTextSize(currentTextSize);
  tft.fillScreen(currentBackgroundColor);
  tft.setTextColor(currentTextColor);
  tft.setCursor(1, 1, currentTextSize);
  tft.setRotation(2);
}

// updateScreen(): updates current screen
void updateScreen(bool display_values) {
  resetScreen();
  if (!display_values) {
    rainbowBackground();
  }
}

// printToScreen(): prints string to the Esp32 screen
void printToScreen(std::string s) { tft.println(getArduinoString(s)); }

// printSensorToScreen(): prints a sensor value to the Esp32 screen
void printSensorToScreen(std::string name, uint8_t value) {
  tft.println(getArduinoString(name));
  tft.println(value);
}

// getArduinoString(): turns a std::string into an Arduino String
String getArduinoString(std::string str) { return (String)str.c_str(); }

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
void rainbowBackground() {
  tft.setRotation(1);
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