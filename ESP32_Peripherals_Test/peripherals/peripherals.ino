/*
 *
 */

int threshold = 40;

const uint8_t buttonPin = 12;        // T5
const uint8_t potentiometerPin = 13; // T4
const uint8_t joystickPinVRx = 27;   // T7
const uint8_t joystickPinVRy = 33;   // T8
const uint8_t joystickPinSW = 32;    // T9

const uint8_t buttonTouchPin = T5;
const uint8_t potentiometerTouchPin = T4;
const uint8_t joystickTouchPinVRx = T7;
const uint8_t joystickTouchPinVRy = T8;
const uint8_t joystickTouchPinSW = T9;

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
void setupPeripherals();
void sendPeripherals();
void gotTouchButton();
void gotTouchPotentiometer();
void gotTouchJoystickVRx();
void gotTouchJoystickVRy();
void gotTouchJoystickSW();
void sendButton();
void sendPotentiometer();
void sendJoystick();
int readPin(int pin);

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

// gotTouchPotentiometer(): updates potentiometer value
void gotTouchPotentiometer() {
  potentiometerDetected = true;
  potentiometerVal = touchRead(potentiometerTouchPin);
}

// gotTouchJoystickVRx(): updates joystick x value
void gotTouchJoystickVRx() {
  joystickDetected = true;
  joystickVRxDetected = true;
  joystickVRxVal = touchRead(joystickTouchPinVRx);
}

// gotTouchJoystickVRy(): updates joystick y value
void gotTouchJoystickVRy() {
  joystickDetected = true;
  joystickVRyDetected = true;
  joystickVRyVal = touchRead(joystickTouchPinVRy);
}

// gotTouchJoystickSW(): updates joystick button value
void gotTouchJoystickSW() {
  joystickDetected = true;
  joystickSWDetected = true;
  joystickSWVal = touchRead(joystickTouchPinSW);
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
    if (joystickVRxDetected) {
      joystickVRxDetected = false;
      Serial.print("<VRx>");
      Serial.print(joystickVRxVal);
      Serial.print("</VRx>");
    }
    if (joystickVRyDetected) {
      joystickVRyDetected = false;
      Serial.print("<VRy>");
      Serial.print(joystickVRyVal);
      Serial.print("</VRy>");
    }
    if (joystickSWDetected) {
      joystickSWDetected = false;
      Serial.print("<SW>");
      Serial.print(joystickSWVal);
      Serial.print("</SW>");
    }
    Serial.print("</joystick>");
  }
}

// readPin(): reads in the value of the specified pin
int readPin(int pin) { return analogRead(pin); }
