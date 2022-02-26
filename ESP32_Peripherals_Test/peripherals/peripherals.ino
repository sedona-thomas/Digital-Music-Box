/*
 *
 */

int threshold = 40;

// main
void setupSerial();
void setupPeripherals();
void sendPeripherals();
int readPin(int pin);

// button
const uint8_t buttonPin = 12;
const uint8_t buttonTouchPin = T5;
bool buttonDetected = false;
uint8_t buttonVal = 0;
void gotTouchButton();
void sendButton();

// potentiometer
const uint8_t potentiometerPin = 13;
const uint8_t potentiometerTouchPin = T4;
bool potentiometerDetected = false;
uint8_t potentiometerVal = 0;
void sendPotentiometer();
void gotTouchPotentiometer();

// joystick
bool joystickDetected = false;
void sendJoystick();

// joystick x
const uint8_t joystickPinVRx = 27;
const uint8_t joystickTouchPinVRx = T7;
bool joystickVRxDetected = false;
uint8_t joystickVRxVal = 0;
void gotTouchJoystickVRx();
void sendJoystickVRx();

// joystick y
const uint8_t joystickPinVRy = 33;
const uint8_t joystickTouchPinVRy = T8;
bool joystickVRyDetected = false;
uint8_t joystickVRyVal = 0;
void gotTouchJoystickVRy();
void sendJoystickVRy();

// joystick button
const uint8_t joystickPinSW = 32;
const uint8_t joystickTouchPinSW = T9;
bool joystickSWDetected = false;
uint8_t joystickSWVal = 0;
void gotTouchJoystickSW();
void sendJoystickSW();

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
  touchAttachInterrupt(buttonTouchPin, gotTouchButton, threshold);
  touchAttachInterrupt(potentiometerTouchPin, gotTouchPotentiometer, threshold);
  touchAttachInterrupt(joystickTouchPinVRx, gotTouchJoystickVRx, threshold);
  touchAttachInterrupt(joystickTouchPinVRy, gotTouchJoystickVRy, threshold);
  touchAttachInterrupt(joystickTouchPinSW, gotTouchJoystickSW, threshold);
}

// sendPeripherals(): sends data from peripherals over the serial connection
void sendPeripherals() {
  Serial.print("<data>");
  sendButton();
  sendPotentiometer();
  sendJoystick();
  Serial.print("</data>");
  Serial.println("");
}

// gotTouchButton(): updates button value
void gotTouchButton() {
  buttonDetected = true;
  buttonVal = touchRead(buttonTouchPin);
}

// sendButton(): sends in the value of the button
void sendButton() {
  if (buttonDetected) {
    buttonDetected = false;
    Serial.print("<button>");
    Serial.print(buttonVal);
    Serial.print("</button>");
  }
}

// gotTouchPotentiometer(): updates potentiometer value
void gotTouchPotentiometer() {
  potentiometerDetected = true;
  potentiometerVal = touchRead(potentiometerTouchPin);
}

// sendPotentiometer(): sends in the value of the potentiometer
void sendPotentiometer() {
  if (potentiometerDetected) {
    potentiometerDetected = false;
    Serial.print("<potentiometer>");
    Serial.print(potentiometerVal);
    Serial.print("</potentiometer>");
  }
}

// sendJoystick(): sends in the value of the joystick
void sendJoystick() {
  if (joystickDetected) {
    joystickDetected = false;
    Serial.print("<joystick>");
    sendJoystickVRx();
    sendJoystickVRy();
    sendJoystickSW();
    Serial.print("</joystick>");
  }
}

// gotTouchJoystickVRx(): updates joystick x value
void gotTouchJoystickVRx() {
  joystickDetected = true;
  joystickVRxDetected = true;
  joystickVRxVal = touchRead(joystickTouchPinVRx);
}

// sendJoystickVRx(): sends in the value of the joystick x direction
void sendJoystickVRx() {
  if (joystickVRxDetected) {
    joystickVRxDetected = false;
    Serial.print("<VRx>");
    Serial.print(joystickVRxVal);
    Serial.print("</VRx>");
  }
}

// gotTouchJoystickVRy(): updates joystick y value
void gotTouchJoystickVRy() {
  joystickDetected = true;
  joystickVRyDetected = true;
  joystickVRyVal = touchRead(joystickTouchPinVRy);
}

// sendJoystickVRy(): sends in the value of the joystick y direction
void sendJoystickVRy() {
  if (joystickSWDetected) {
    joystickSWDetected = false;
    Serial.print("<SW>");
    Serial.print(joystickSWVal);
    Serial.print("</SW>");
  }
}

// gotTouchJoystickSW(): updates joystick button value
void gotTouchJoystickSW() {
  joystickDetected = true;
  joystickSWDetected = true;
  joystickSWVal = touchRead(joystickTouchPinSW);
}

// sendJoystickSW(): sends in the value of the joystick button
void sendJoystickSW() {
  if (joystickSWDetected) {
    joystickSWDetected = false;
    Serial.print("<SW>");
    Serial.print(joystickSWVal);
    Serial.print("</SW>");
  }
}

// readPin(): reads in the value of the specified pin
int readPin(int pin) { return analogRead(pin); }
