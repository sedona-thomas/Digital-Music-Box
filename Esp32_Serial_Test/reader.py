#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial

port = '/dev/cu.usbserial-023E564D' # esp32
baudrate = 115200

s = serial.Serial(port, baudrate)

while(True):
  get_sensor_value()
  print()


def get_sensor_value():
  sensor = s.readline().strip()
  return str(sensor, 'ascii')

