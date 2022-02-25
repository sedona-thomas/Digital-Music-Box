#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
    Setup:
        button = {"+": "3V", "-" : "12"}
        potentiometer = {"in": "3V", "out": "13", "ground": "G"}
        joystick = {"ground": "G", "5V": "5V", "VRx": "27", "VRy": "26", "SW": "25"}
'''

import serial
from html.parser import HTMLParser


class SerialPeripheralParser(HTMLParser):

    def handle_starttag(self, tag, attrs):
        print("Encountered a start tag:", tag)

    def handle_endtag(self, tag):
        print("Encountered an end tag :", tag)

    def handle_data(self, data):
        print("Encountered some data  :", data)


class DisplayWithPeripherals():

    def __init__():
        self.port = '/dev/cu.usbserial-023E564D'  # esp32
        self.baudrate = 115200
        self.s = serial.Serial(port, baudrate)
        self.button_val = 0
        self.potentiometer_val = 0
        self.joystick_vals = {"VRx": 0, "VRy": 0, "SW": 0}
        self.parser = SerialPeripheralParser()

    def update(self):
        sensor = s.readline().strip()
        sensor_str = str(sensor, 'ascii')
        parser.feed(sensor_str)
        return

    def get_button(self):
        return button_val

    def get_potentiometer(self):
        return potentiometer_val

    def get_joystick(self):
        return joystick_vals


if __name__ == "__main__":
    display = DisplayWithPeripherals()
    while(True):
        display.update()
