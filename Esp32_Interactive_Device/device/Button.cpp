/*
 * Button.cpp: Button class methods
 */

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
