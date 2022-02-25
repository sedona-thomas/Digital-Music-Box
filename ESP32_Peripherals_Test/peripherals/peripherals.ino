#include <unordered_map>

// peripherals
int buttonPin = 12;        // T5
int potentiometerPin = 13; // T4
std::unordered_map<std::string, int> joystickPins = {
    {"VRx", 27}, // T9
    {"VRy", 33}, // T8
    {"SW", 32}   // T7
};

int threshold = 40;
bool buttonDetected = false;
bool potentiometerDetected = false;
std::unordered_map<std::string, bool> joystickDetected = {
    {"VRx", false}, {"VRy", false}, {"SW", false}};
uint8_t touch1Val = 0;
uint8_t touch2Val = 0;

// main
void setupSerial();
void readPin(int pin);
void sendButton();
void sendPotentiometer();
void sendJoystick();

// professor Santolucito's
// int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
uint8_t touch1Val = 0;
uint8_t touch2Val = 0;
void setupTest();
void loopTest();
void gotTouch1();
void gotTouch2();

void setup() {
  setupSerial();

  setupTest();
}

void loop() {
  delay(100);

  loopTest();
}

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
}

// setupPeripherals(): starts serial communication
void setupPeripherals() {
  touchAttachInterrupt(T2, gotTouch1, threshold);
  touchAttachInterrupt(T9, gotTouch2, threshold);
}

// sendPeripherals(): sends data from peripherals over the serial connection
void sendPeripherals() {
  Serial.println("<start>");
  sendButton();
  sendPotentiometer();
  sendJoystick();
  Serial.println("<stop>");
}

// readPin(): reads in the value of the specified pin
int readPin(int pin) { return analogRead(pin); }

// sendButton(): sends in the value of the button
void sendButton() {
  Serial.println("<button>");

  readPin();
  Serial.println();

  Serial.println("</button>");
}

// sendPotentiometer(): sends in the value of the potentiometer
void sendPotentiometer() {
  Serial.println("<potentiometer>");

  int pinValue = analogRead(pin);

  Serial.println("</potentiometer>");
}

// sendJoystick(): sends in the value of the joystick
void sendJoystick() {
  Serial.println("<joystick>");

  int value = analogRead(pin);

  Serial.println("</joystick>");
}

// professor Santolucito's

void setupTest() {
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T2, gotTouch1, threshold);
  touchAttachInterrupt(T9, gotTouch2, threshold);
}

void loopTest() {
  if (touch1detected) {
    touch1detected = false;
    Serial.println(touch1Val);
  }
  if (touch2detected) {
    touch2detected = false;
    Serial.println(touch2Val);
  }
}

void gotTouch1() {
  touch1detected = true;
  touch1Val = touchRead(T2);
}

void gotTouch2() {
  touch2detected = true;
  touch2Val = touchRead(T9);
}
