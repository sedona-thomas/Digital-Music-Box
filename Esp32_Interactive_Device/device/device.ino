/*
 *
 */

#include <SPI.h>
#include <TFT_eSPI.h>
#include <list>
#include <stdint.h>
#include <string>

#define WAIT 500     // miliseconds
#define FRAMERATE 50 // miliseconds

TFT_eSPI tft = TFT_eSPI();
uint32_t currentBackgroundColor = TFT_WHITE, currentTextColor = TFT_BLACK;
uint8_t currentTextSize = 1; // 10 pixels

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

Button::Button(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  value = 0;
  json = json_in;
  for (int i = 0; i < 8; i++) {
    values.push_back(0);
  }
}

Button::Button(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
  json = json_in;
  for (int i = 0; i < 8; i++) {
    values.push_back(0);
  }
}

// read(): reads button value
void Button::read() {
  pinMode(pin, INPUT_PULLUP);
  values.push_back(digitalRead(pin));
  values.pop_front();
  int sum = 0;
  for (auto val : values) {
    sum += val;
  }
  value = sum / values.size() < 0.1 ? 0 : 1;
  value = (std::find(values.begin(), values.end(), 1) != values.end());
  tft.println("button");
  tft.println(value);
};

// send(): sends data from peripheral over the serial connection
void Button::send() {
  Button::read();
  if (json) {
    Serial.print("button_");
    Serial.print(name.c_str());
    Serial.print(": ");
    Serial.print(value);
    Serial.print(",");
  } else {
    Serial.print("<button_");
    Serial.print(name.c_str());
    Serial.print(">");
    Serial.print(value);
    Serial.print("</button_");
    Serial.print(name.c_str());
    Serial.print(">");
  }
}

class Potentiometer {
public:
  std::string name;
  uint8_t pin;
  uint8_t value;
  bool json;

  Potentiometer() {}
  Potentiometer(std::string, int, bool);
  Potentiometer(int, bool);
  void read();
  void send();
};

Potentiometer::Potentiometer(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  value = 0;
  json = json_in;
}

Potentiometer::Potentiometer(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  value = 0;
  json = json_in;
}

// read(): reads potentiometer value
void Potentiometer::read() {
  value = analogRead(pin);
  tft.println("potentiometer");
  tft.println(value);
};

// send(): sends data from peripheral over the serial connection
void Potentiometer::send() {
  Potentiometer::read();
  if (json) {
    if (name.length() > 0) {
      Serial.print("potentiometer_");
      Serial.print(name.c_str());
      Serial.print(": ");
      Serial.print(value);
      Serial.print(",");
    } else {
      Serial.print("potentiometer: ");
      Serial.print(value);
      Serial.print(",");
    }
  } else {
    if (name.length() > 0) {
      Serial.print("<potentiometer_");
      Serial.print(name.c_str());
      Serial.print(">");
      Serial.print(value);
      Serial.print("</potentiometer_");
      Serial.print(name.c_str());
      Serial.print(">");
    } else {
      Serial.print("<potentiometer>");
      Serial.print(value);
      Serial.print("</potentiometer>");
    }
  }
};

class Joystick {
public:
  std::string name;
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;
  bool json;

  Joystick() {}
  Joystick(std::string, int, int, int, bool);
  void read();
  void send();
};

Joystick::Joystick(std::string name_in, int pin_X, int pin_Y, int pin_SW,
                   bool json_in) {
  name = name_in;
  potentiometerX = Potentiometer("_x" + name_in, pin_X, json_in);
  potentiometerY = Potentiometer("_y" + name_in, pin_Y, json_in);
  buttonSW = Button("_sw" + name_in, pin_SW, json_in);
  json = json_in;
}

// read(): reads joystick value
void Joystick::read() {
  tft.println("joystick");
  potentiometerX.read();
  potentiometerY.read();
  buttonSW.read();
};

// send(): sends data from peripheral over the serial connection
void Joystick::send() {
  if (json) {
    Serial.print("joystick_");
    Serial.print(name.c_str());
    Serial.print(": ");
    Serial.print("{");
    potentiometerX.send();
    potentiometerY.send();
    buttonSW.send();
    Serial.print("},");
  } else {
    Serial.print("<joystick_");
    Serial.print(name.c_str());
    Serial.print(">");
    potentiometerX.send();
    potentiometerY.send();
    buttonSW.send();
    Serial.print("</joystick_");
    Serial.print(name.c_str());
    Serial.print(">");
  }
};

/*
 * Main
 */

unsigned long startTime = 0, loopStartTime = 0;

bool json = false;
Button button = Button("button1", 37, json);
Potentiometer potentiometer = Potentiometer("potentiometer1", 12, json);
Joystick joystick = Joystick("joystick1", 27, 26, 25, json);

void setupScreen();
void setupSerial();
void sendPeripherals();
void updateScreen();
void resetScreen();
std::vector<String> getLetterVector(std::string str);
inline uint16_t randomColor();
inline uint16_t getRGB(uint8_t r, uint8_t g, uint8_t b);
void rainbowBackground();

void setup() {
  setupScreen();
  setupSerial();
}

void loop() {
  updateScreen();
  sendPeripherals();
  delay(FRAMERATE);
}

// setupScreen(): starts ESP32 screen
void setupScreen() {
  tft.init();
  tft.setRotation(2);
  startTime = millis();
}

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000);
}

// sendPeripherals(): sends values of all peripherals
void sendPeripherals() {
  if (json) {
    Serial.print("{ data:");
    button.send();
    potentiometer.send();
    joystick.send();
    Serial.println("}");
  } else {
    Serial.print("<data>");
    button.send();
    potentiometer.send();
    joystick.send();
    Serial.println("</data>");
  }
}

// updateScreen(): updates current screen
void updateScreen() {
  loopStartTime = millis();
  resetScreen();
}

// resetScreen(): resets the background and text color/size of the display
void resetScreen() {
  tft.setTextSize(currentTextSize);
  tft.fillScreen(currentBackgroundColor);
  tft.setTextColor(currentTextColor);
  tft.setCursor(0, 0, currentTextSize);
}

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