# TTGO-Interactive-Device

## Materials

 - ESP32 TTGO T-Display (CP 2104 Chip)
 - computer
 - USB-C cord
 - button
 - joystick
 - potentiometer
 - wires
 - 1/8 in acrylic sheet
 - acrylic glue
 - hot glue
 - wood blocks
 - soldering tools
 - laser cutter

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

#### Upload Esp32 Serial Communication

- Open the Arduino IDE
- Open `Esp32_Serial_Communication/device/device.ino` from the Arduino IDE
- Plug in the ESP32 TTGO T-Display
- Select `Sketch > Upload`

### Build Hardware

#### Solder Button

- Solder the `+` (voltage in) end of the button to a ground pin of the ESP32
- Solder the `-` (voltage out) end of the button to the `GPIO Pin #37` of the ESP32

#### Solder Potentiometer

- Solder the `V_in` (voltage in, the first pin) end of the potentiometer to a 3V pin of the ESP32 (the 3V pin on the bottom left)
- Solder the `V_out` (voltage out, the second pin) end of the potentiometer to the `GPIO Pin #13` of the ESP32
- Solder the `GND` (ground, the third pin) end of the potentiometer to a ground pin of the ESP32 (the second ground pin on the top left)

#### Solder Joystick

- Solder the `GND` (ground, the first pin) end of the potentiometer to a ground pin of the ESP32 (the ground pin on the bottom right)
- Solder the `+5V` (voltage in, the second pin) end of the potentiometer to a 3V pin of the ESP32 (the 3V pin on the top right)
- Solder the `VRx` (x-axis potentiometer, the third pin) end of the potentiometer to the `GPIO Pin #27` of the ESP32
- Solder the `VRy` (y-axis potentiometer, the fourth pin) end of the potentiometer to the `GPIO Pin #26` of the ESP32
- Solder the `SW` (button, the fifth pin) end of the potentiometer to the `GPIO Pin #25` of the ESP32

### Make Case

#### Cut Case

- Download `Case/caseplans.svg`
- Upload file to a laser cutter
- Choose a sheet of 1/8 in acrylic (can also be made of cardboard or wood)
- Cut out the pattern
- Drill a hole in the side of the box that fits your USB-C cable so the Esp32 can be plugged in
- Cut a 2.25 in block of wood for the Esp32
- Cut a 1.50 in block of wood for the joystick
- Cut a 2.00 in block of wood for the potentiometer

#### Assemble Case

- Use acrylic glue to assemble the box, but do not attach the bottom panel with the single hole in the corner
- Use sandpaper to remove all harsh edges on the blocks of wood
- Hot glue the joystick and potentiometer to their blocks of wood
- Screw the button into place (if button is too small, add a small zip tie before screwing on)
- Hot glue each block of wood so the corresponding peripheral fits in the correct sized hole
- Place the Esp32 over its block, plug it in, and carefully close the box

### Run Device

- Plug the device into your computer
- Open `Digital_Music_Box/index.html` in the browser of your choice
- Play your digital synthesizer and control it with your music box 

## More Information

Medium Blog Post: [https://medium.com/@sedonathomas/interactive-device-3c5826457c24](https://medium.com/@sedonathomas/interactive-device-3c5826457c24)

Youtube Demo: 

##
## Special Thanks to Professor Mark Santolucito and the staff at the Barnard Design Center for all of their help!!!
