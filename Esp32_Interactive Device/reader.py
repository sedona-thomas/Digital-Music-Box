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
        self.tags = {"data": self.process_data,
                     "button": self.process_button,
                     "potentiometer": self.process_potentiometer,
                     "joystick": self.process_joystick,
                     "VRx": self.process_VRx,
                     "VRy": self.process_VRy,
                     "SW": self.process_SW}
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
        Executes the proper function for the current tag

        @param tag the tag associated with the data
        @param data the data contained in the tag
    """
    def process_tag(self, tag, data):
        if tag in self.tags.keys():
            self.tags[tag](data)

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
        Processes the data contained within the <button> tag

        @param data the data within the <button> tag
    """
    def process_button(self, data):
        if data.strip().isnumeric():
            self.device.button_val = int(data)
            print("b", data)

    """
        Processes the data contained within the <potentiometer> tag

        @param data the data within the <potentiometer> tag
    """
    def process_potentiometer(self, data):
        if data.strip().isnumeric():
            self.device.potentiometer_val = int(data)
            print("p", data)

    """
        Processes the data contained within the <joystickr> tag

        @param data the data within the <joystick> tag
    """
    def process_joystick(self, data):
        remaining = data
        while remaining:
            tag, data, remaining = self.parse(remaining)
            if tag:
                self.process_tag(tag, data)

    """
        Processes the data contained within the <VRx> tag

        @param data the data within the <VRx> tag
    """
    def process_VRx(self, data):
        if data.strip().isnumeric():
            self.device.joystick_vals["VRx"] = int(data)
            print("VRx", data)

    """
        Processes the data contained within the <VRy> tag

        @param data the data within the <VRy> tag
    """
    def process_VRy(self, data):
        if data.strip().isnumeric():
            self.device.joystick_vals["VRy"] = int(data)
            print("VRy", data)

    """
        Processes the data contained within the <SW> tag

        @param data the data within the <SW> tag
    """
    def process_SW(self, data):
        if data.strip().isnumeric():
            self.device.joystick_vals["SW"] = int(data)
            print("SW", data)


class DisplayWithPeripherals(object):

    """
        The DisplayWithPeripherals tracks and updates sensor values for an ESP32 with peripherals

        @param device the object for which to update values
    """
    def __init__(self):
        self.port = '/dev/cu.usbserial-023E564D'  # esp32
        self.baudrate = 115200
        self.s = serial.Serial(self.port, self.baudrate)
        self.button_val = 0
        self.potentiometer_val = 0
        self.joystick_vals = {"VRx": 0, "VRy": 0, "SW": 0}
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
