/*
 * This is an example how to use Touch Intrrerupts and read touch values
 * 
 * @author Professor Mark Santolucito
 * @link https://gist.github.com/santolucito/44410ed78def1b68b9994b74227f59ee
 */

int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
uint8_t touch1Val = 0;
uint8_t touch2Val = 0;

void gotTouch1(){
 touch1detected = true;
 touch1Val = touchRead(T2);
}

void gotTouch2(){
 touch2detected = true;
 touch2Val = touchRead(T9);
}

void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T2, gotTouch1, threshold);
  touchAttachInterrupt(T9, gotTouch2, threshold);
}

void loop(){
  if(touch1detected){
    touch1detected = false;
    Serial.println(touch1Val);
  }
  if(touch2detected){
    touch2detected = false;
    Serial.println(touch2Val);
  }
}


//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(115200);
//  Serial.println("Hello, ESP32!");
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  delay(10); // this speeds up the simulation
//  Serial.println(analogRead(27));
//}
