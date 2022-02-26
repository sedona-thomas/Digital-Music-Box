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
        if "data" not in tag.lower():
            print("Encountered a start tag:", tag)

    def handle_endtag(self, tag):
        if "data" not in tag.lower():
            print("Encountered an end tag :", tag)

    def handle_data(self, data):
        print("Encountered some data  :", data)


class PeripheralTagProcessor(object):

    def __init__(self):
        self.tags = {"data": self.process_data, "button": self.process_button, "potentiometer": self.process_potentiometer,
                     "joystick": self.process_joystick, "VRx": self.process_VRx, "VRy": self.process_VRy, "SW": self.process_SW}

    def process_tag(self, tag):
        self.tags[tag]

    def process_data(self):
        pass

    def process_button(self):
        pass

    def process_potentiometer(self):
        pass

    def process_joystick(self):
        pass

    def process_VRx(self):
        pass

    def process_VRy(self):
        pass

    def process_SW(self):
        pass


class DisplayWithPeripherals(object):

    def __init__(self):
        self.port = '/dev/cu.usbserial-023E564D'  # esp32
        self.baudrate = 115200
        self.s = serial.Serial(self.port, self.baudrate)
        self.button_val = 0
        self.potentiometer_val = 0
        self.joystick_vals = {"VRx": 0, "VRy": 0, "SW": 0}
        self.parser = SerialPeripheralParser()

    def update(self):
        sensor = str(self.s.readline(), 'ascii').strip()
        self.parser.feed(sensor)
        return

    def get_button(self):
        return self.button_val

    def get_potentiometer(self):
        return self.potentiometer_val

    def get_joystick(self):
        return self.joystick_vals


if __name__ == "__main__":
    display = DisplayWithPeripherals()
    while(True):
        display.update()
