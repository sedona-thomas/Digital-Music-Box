# this code will read the touch values from the ESP32 and echo them on the command line
# you could do something else more interesting with these values (e.g. visualize/sonify)

import serial

esp32_port = '/dev/cu.usbserial-023E564D'

ser = serial.Serial(esp32_port, 115200)

while(True):
  print(str(ser.readline().strip(), 'ascii'))

