int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
uint8_t touch1Val = 0;
uint8_t touch2Val = 0;

// main
void setupSerial();
void readPin(int pin);

// professor Santolucito's
void setupTest();
void loopTest();
void gotTouch1();
void gotTouch2();


void setup() {
  setupSerial();
  setupTest();
}

void loop(){
  loopTest();
}

// setupSerial(): starts serial communication
void setupSerial() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
}

// readPin(): reads in the value of the specified pin
void readPin(int pin) {
  int pinValue = analogRead(pin);
  Serial.println(pinValue);
}


// professor Santolucito's

void setupTest() {
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T2, gotTouch1, threshold);
  touchAttachInterrupt(T9, gotTouch2, threshold);
}

void loopTest() {
  if(touch1detected){
    touch1detected = false;
    Serial.println(touch1Val);
  }
  if(touch2detected){
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
