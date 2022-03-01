#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    This code will accept serial sensor data from multiple sensors attached to an ESP32

    @Author: Sedona Thomas
    @Date: 02/25/2022
    @Links: https://github.com/sedona-thomas/TTGO-Interactive-Device
"""

import serial
import re


class PeripheralTagParser(object):

    """
        The PeripheralTagParser parses sensor data inputted as HTML tags for special tag names

        @param device the object for which to update values
    """
   def __init__(self, device):
        self.tags = ["data"] + list(device.values.keys())
        self.device = device

    """
        Feeds in a string to be parsed

        @param text the string with tags
    """
    def feed(self, text):
        self.process_data(text)

    """
        Parses a string with tags

        @param text the string with tags
        @return tag, data within tag, remaining string to process
    """
    def parse(self, text):
        text = text.strip()
        tag, open_tag, close_tag = self.extract_current_tag(text)
        if tag:
            data = text[open_tag[1]: close_tag[0]]
            remaining = text[close_tag[0]:]
            return tag, data, remaining
        else:
            return tag, "", ""

    """
        Extracts the next tag and returns its start and end positions

        @param text the string with tags
        @return tag as string, span of opening tag, span of closing tag
    """
    def extract_current_tag(self, text):
        open_tag = re.search("<\s*\w*\s*>", text)
        if open_tag:
            tag = re.search("\w+", open_tag.group(0))
            if tag:
                close_tag = re.search("</\s*" + tag.group(0) + "*\s*>", text)
                if close_tag:
                    return tag.group(0), open_tag.span(), close_tag.span()
        return "", (-1, -1), (-1, -1)


    """
        Processes the data contained within the <data> tag

        @param data the data within the <data> tag
    """
    def process_data(self, data):
        remaining = data
        while remaining:
            tag, data, remaining = self.parse(remaining)
            self.process_tag(tag, data)

    """
        Processes the data contained within the given tag

        @param data the data within the given tag
    """
    def process_tag(self, tag, data):
        if data.strip().isnumeric():
            self.device.values[tag] = int(data)
            print(tag, data)



class DisplayWithPeripherals(object):

    """
        The DisplayWithPeripherals tracks and updates sensor values for an ESP32 with peripherals

        @param device the object for which to update values
    """
    def __init__(self):
        self.port = '/dev/cu.usbserial-023E564D'  # esp32
        self.baudrate = 115200
        self.s = serial.Serial(self.port, self.baudrate)
        self.values = {"button": 0, "potentiometer": 0, "joystickVRx": 0, "joystickVRy": 0, "joystickSW": 0}
        self.parser = PeripheralTagParser(self)

    """
        Updates the current sensor values
    """
    def update(self):
        sensor = str(self.s.readline(), 'ascii').strip()
        self.parser.feed(sensor)

    """
        Returns the current button value

        @return current button value
    """
    def get_button(self):
        return self.button_val

    """
        Returns the current potentiometer value

        @return current potentiometer value
    """
    def get_potentiometer(self):
        return self.potentiometer_val

    """
        Returns the current joystick values

        @return current joystick values
    """
    def get_joystick(self):
        return self.joystick_vals


if __name__ == "__main__":
    display = DisplayWithPeripherals()
    while(True):
        display.update()




'''

self.tags = {"data": self.process_data,
                     "button": self.process_button,
                     "potentiometer": self.process_potentiometer,
                     "joystickVRx": self.process_joystickVRx,
                     "joystickVRy": self.process_joystickVRy,
                     "joystickSW": self.process_joystickSW}

    """
        Executes the proper function for the current tag

        @param tag the tag associated with the data
        @param data the data contained in the tag
    """
    def process_tag(self, tag, data):
        if tag in self.tags.keys():
            self.tags[tag](data)


    """
        Processes the data contained within the <button> tag

        @param data the data within the <button> tag
    """
    def process_button(self, data):
        if data.strip().isnumeric():
            self.device.values["button"] = int(data)
            print("b", data)

    """
        Processes the data contained within the <potentiometer> tag

        @param data the data within the <potentiometer> tag
    """
    def process_potentiometer(self, data):
        if data.strip().isnumeric():
            self.device.values["potentiometer"] = int(data)
            print("p", data)

    """
        Processes the data contained within the <joystickVRx> tag

        @param data the data within the <joystickVRx> tag
    """
    def process_joystickVRx(self, data):
        if data.strip().isnumeric():
            self.device.values["joystickVRx"] = int(data)
            print("joystickVRx", data)

    """
        Processes the data contained within the <joystickVRy> tag

        @param data the data within the <joystickVRy> tag
    """
    def process_VRy(self, data):
        if data.strip().isnumeric():
            self.device.values["joystickVRy"] = int(data)
            print("joystickVRy", data)

    """
        Processes the data contained within the <joystickSW> tag

        @param data the data within the <joystickSW> tag
    """
    def process_SW(self, data):
        if data.strip().isnumeric():
            self.device.values["joystickSW"] = int(data)
            print("joystickSW", data)
'''