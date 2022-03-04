/*
 *
 */

class Button {
public:
  std::string name;
  uint8_t pin;
  bool detected;
  uint8_t value;
  bool json;

  Button(std::string, int, bool);
  Button(int, bool);
  void read();
  void send();
};

Button::Button(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  detected = false;
  value = 0;
  json = json_in;
}

Button::Button(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  detected = false;
  value = 0;
  json = json_in;
}

// read(): reads button value
void Button::read() {
  detected = true;
  pinMode(pin, INPUT_PULLUP);
  digitalRead(pin);
};

// send(): sends data from peripheral over the serial connection
void Button::send() {
  if (detected) {
    detected = false;
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
};

class Potentiometer {
public:
  std::string name;
  uint8_t pin;
  bool detected;
  uint8_t value;
  bool json;

  Potentiometer(std::string, int, bool);
  Potentiometer(int, bool);
  void read();
  void send();
};

Potentiometer::Potentiometer(std::string name_in, int pin_in, bool json_in) {
  name = name_in;
  pin = pin_in;
  detected = false;
  value = 0;
  json = json_in;
}

Potentiometer::Potentiometer(int pin_in, bool json_in) {
  name = "";
  pin = pin_in;
  detected = false;
  value = 0;
  json = json_in;
}

// read(): reads potentiometer value
void Potentiometer::read() {
  detected = true;
  analogRead(pin);
};

// send(): sends data from peripheral over the serial connection
void Potentiometer::send() {
  if (detected) {
    detected = false;
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
  }
};

class Joystick {
public:
  std::string name;
  Potentiometer potentiometerX;
  Potentiometer potentiometerY;
  Button buttonSW;
  bool json;

  Joystick(std::string, int, int, int, bool);
  void read();
  void send();
};

Joystick::Joystick(std::string name_in, int pin_X, int pin_Y, int pin_SW,
                   bool json_in) {
  name = name_in;
  potentiometerX = Potentiometer(pin_X, json_in);
  potentiometerY = Potentiometer(pin_Y, json_in);
  buttonSW = Button(pin_SW, json_in);
  json = json_in;
}

// read(): reads joystick value
void Joystick::read() {
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
    Serial.print(value);
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
   * Main Code
   */

#define WAIT 500      // miliseconds
#define FRAMERATE 100 // miliseconds

#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdint.h>
TFT_eSPI tft = TFT_eSPI();

uint32_t currentBackgroundColor = TFT_WHITE;
uint32_t currentTextColor = TFT_BLACK;
uint8_t currentTextSize = 1; // 10 pixels

byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11;

unsigned long startTime = 0;
unsigned long loopStartTime = 0; // Used for testing draw times

Button button;
Potentiometer potentiometer;
Joystick joystick;

// setupScreen(): starts ESP32 screen
void setupScreen() {
  tft.init();
  tft.setRotation(1);
  startTime = millis();
}

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000);
  setupPeripherals();
}

// setupPeripherals(): sets up connections to all peripherals
void setupPeripherals() {
  bool json = false;
  button = Button("button1", 37, json);
  potentiometer = Potentiometer("potentiometer1", 12, json);
  joystick = Joystick("joystick1", 27, 26, 25, json);
}

// updateScreen(): updates current screen
void updateScreen() {
  loopStartTime = millis();
  resetScreen();
  tft.println("Hello World!");
  delay(FRAMERATE);
}

// sendPeripherals(): sends values of all peripherals
void sendPeripherals() {
  Serial.print("{ data:");
  button.send();
  potentiometer.send();
  joystick.send();
  Serial.println("}");
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

// randomColor(): outputs a random pastel color that can be used for a
// background
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

void setup() {
  setupScreen();
  setupSerial();
}

void loop() {
  updateScreen();
  sendPeripherals();
}