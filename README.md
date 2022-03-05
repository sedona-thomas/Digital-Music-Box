# TTGO-Interactive-Device

## Materials

 - ESP32 TTGO T-Display (CP 2104)
 - Computer
 - USB-C cord
 - button
 - joystick
 - potentiometer
 - wires
 - soldering tools
 - Acrylic sheet
 - Laser cutter

## Make at Home

### Setup Arduino

- Download and open the Arduino IDE ([Download](https://www.arduino.cc/en/software))
- Open Preferences 
- Copy and paste the following link ([https://dl.espressif.com/dl/package_esp32_index.json](https://dl.espressif.com/dl/package_esp32_index.json)) into `Additonal Boards Manager URLs` to add the ESP package
- Select `Tools > Boards > Boards Manager`, then search for `esp32` and install the most recent version
- Select `Tools > Boards > ESP32 Arduino > TTGO T1`
- Select `Tools > Manage Libraries`, then search for `TFT_eSPI` and install the most recent version
- In your file storage system, find the Arduino folder
- Open `Arduino/libraries/TFT_eSPI/User_Setup_Select.h`
- Comment out the line that says `#include <User_Setup.h>`
- Uncomment the line that says `#include <User_Setups/Setup25_TTGO_T_Display.h>`

#### Test ESP32 TTGO T-Display

- Open the Arduino IDE
- Select `Files > Examples > TFT_eSPI > 160 x 128 > RLE_font_test`
- Plug in the ESP32 TTGO T-Display
- Select `Sketch > Upload`

#### Setup Python Serial Ports

- Download Anaconda ([https://www.anaconda.com/products/individual](https://www.anaconda.com/products/individual))
- Open Terminal
- Download pyserial `conda install pyserial`

### Test ESP32 Inputs

- 

### Upload Sensor Program

- 

### Build Hardware

#### Solder Button

- Solder the `+` (voltage in) end of the button to a 3V pin of the ESP32
- Solder the `-` (voltage out) end of the button to the `GPIO Pin #37` of the ESP32

#### Solder Potentiometer

- Solder the `V_in` (voltage in, the first pin) end of the potentiometer to a 3V pin of the ESP32
- Solder the `V_out` (voltage out, the second pin) end of the potentiometer to the `GPIO Pin #12` of the ESP32
- Solder the `GND` (ground, the third pin) end of the potentiometer to a ground pin of the ESP32

#### Solder Joystick

- Solder the `GND` (ground, the first pin) end of the potentiometer to a ground pin of the ESP32
- Solder the `+5V` (voltage in, the second pin) end of the potentiometer to a 5V pin of the ESP32
- Solder the `VRx` (x-axis potentiometer, the third pin) end of the potentiometer to the `GPIO Pin #27` of the ESP32
- Solder the `VRy` (y-axis potentiometer, the fourth pin) end of the potentiometer to the `GPIO Pin #26` of the ESP32
- Solder the `SW` (button, the fifth pin) end of the potentiometer to the `GPIO Pin #25` of the ESP32

### Make Case

## Cut Case

- 

## Assemble Case

- 

### Run Device

- Plug the device into your computer
- Run `./_______.py`


## More Information

Medium Blog Post: [https://medium.com/@sedonathomas/interactive-device-3c5826457c24](https://medium.com/@sedonathomas/interactive-device-3c5826457c24)

Youtube Demo: 

##
## Special Thanks to Professor Mark Santolucito and the staff at the Barnard Design Center for all of their help!!!
