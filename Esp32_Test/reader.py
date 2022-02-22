#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
  this code will read the touch values from the ESP32 and echo them on the command line
  you could do something else more interesting with these values (e.g. visualize/sonify)
  @author Professor Mark Santolucito
  @link https://gist.github.com/santolucito/44410ed78def1b68b9994b74227f59ee
'''


import serial

esp32_port = '/dev/cu.usbserial-023E564D'

ser = serial.Serial(esp32_port, 115200)

while(True):
  print(str(ser.readline().strip(), 'ascii'))

