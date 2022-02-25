/*
 *
 */

// peripherals
const uint8_t buttonPin = 12;        // T5
const uint8_t potentiometerPin = 13; // T4
const uint8_t joystickPinVRx = 27;   // T9
const uint8_t joystickPinVRy = 33;   // T8
const uint8_t joystickPinSW = 32;    // T7

int threshold = 40;

bool buttonDetected = false;
bool potentiometerDetected = false;
bool joystickDetected = false;
bool joystickVRxDetected = false;
bool joystickVRyDetected = false;
bool joystickSWDetected = false;

uint8_t buttonVal = 0;
uint8_t potentiometerVal = 0;
uint8_t joystickVRxVal = 0;
uint8_t joystickVRyVal = 0;
uint8_t joystickSWVal = 0;

// main
void setupSerial();
int readPin(int pin);
void sendButton();
void sendPotentiometer();
void sendJoystick();

void setup() { setupSerial(); }

void loop() { sendPeripherals(); }

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  setupPeripherals();
}

// setupPeripherals(): starts serial communication
void setupPeripherals() {
  touchAttachInterrupt(T5, gotTouchButton, threshold);
  touchAttachInterrupt(T4, gotTouchPotentiometer, threshold);
  touchAttachInterrupt(T9, gotTouchJoystickVRx, threshold);
  touchAttachInterrupt(T8, gotTouchJoystickVRy, threshold);
  touchAttachInterrupt(T7, gotTouchJoystickSW, threshold);
}

// sendPeripherals(): sends data from peripherals over the serial connection
void sendPeripherals() {
  Serial.println("<data>");
  sendButton();
  sendPotentiometer();
  sendJoystick();
  Serial.println("</data>");
}

void gotTouchButton() {
  buttonDetected = true;
  buttonVal = touchRead(T5);
}

void gotTouchPotentiometer() {
  potentiometerDetected = true;
  potentiometerVal = touchRead(T4);
}

void gotTouchJoystickVRx() {
  joystickDetected = true;
  joystickVRxDetected = true;
  joystickVRxVal = touchRead(T9);
}

void gotTouchJoystickVRy() {
  joystickDetected = true;
  joystickVRyDetected = true;
  joystickVRyVal = touchRead(T8);
}

void gotTouchJoystickSW() {
  joystickDetected = true;
  joystickSWDetected = true;
  joystickSWVal = touchRead(T7);
}

// sendButton(): sends in the value of the button
void sendButton() {
  if (buttonDetected) {
    buttonDetected = false;
    Serial.println("<button>");
    Serial.println(buttonVal);
    Serial.println("</button>");
  }
}

// sendPotentiometer(): sends in the value of the potentiometer
void sendPotentiometer() {
  if (potentiometerDetected) {
    potentiometerDetected = false;
    Serial.println("<potentiometer>");
    Serial.println(potentiometerVal);
    Serial.println("</potentiometer>");
  }
}

// sendJoystick(): sends in the value of the joystick
void sendJoystick() {
  if (joystickDetected) {
    joystickDetected = false;
    Serial.println("<joystick>");
    if (joystickVRxDetected) {
      joystickVRxDetected = false;
      Serial.println("<VRx>");
      Serial.println(joystickVRxVal);
      Serial.println("</VRx>");
    }
    if (joystickVRyDetected) {
      joystickVRyDetected = false;
      Serial.println("<VRy>");
      Serial.println(joystickVRyVal);
      Serial.println("</VRy>");
    }
    if (joystickSWDetected) {
      joystickSWDetected = false;
      Serial.println("<SW>");
      Serial.println(joystickSWVal);
      Serial.println("</SW>");
    }
    Serial.println("</joystick>");
  }
}

// readPin(): reads in the value of the specified pin
int readPin(int pin) { return analogRead(pin); }

/*

//  *
//  * professor Santolucito's example
//  *


// int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
uint8_t touch1Val = 0;
uint8_t touch2Val = 0;
void setupTest();
void loopTest();
void gotTouch1();
void gotTouch2();

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
*/