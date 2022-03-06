#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    This code will accept serial sensor data from multiple sensors attached to an ESP32

    @Author: Sedona Thomas
    @Date: 02/25/2022
    @Links: https://github.com/sedona-thomas/TTGO-Interactive-Device
"""

import serial
import json
import re
from collections import defaultdict


class TagParser(object):

    """
    The TagParser parses sensor data inputted as HTML tags for special tag names

    :param :device the object for which to update values
    :returns: returns nothing
    """

    def __init__(self, device):
        self.tags = ["data"] + list(device.values.keys())
        self.device = device

    """
    Feeds in a string to be parsed

    :param :text the string with tags
    :returns: returns nothing
    """

    def feed(self, text):
        self.process_data(text)

    """
    Processes the data contained within the next tag

    :param :data the text data within the next tag
    :returns: returns nothing
    """

    def process_data(self, data):
        remaining = data
        while remaining:
            tag, data, remaining = self.parse(remaining)
            self.process_tag(tag, data)

    """
    Parses a string with tags

    :param :text the string with tags
    :returns: returns tag, data within tag, remaining string to process
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

    :param :text the string with tags
    :returns: returns tag as string, span of opening tag, span of closing tag
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
    Processes the data contained within the given tag

    :param :data the data within the given tag
    :returns: returns nothing
    """

    def process_tag(self, tag, data):
        if data.strip().isnumeric():
            self.device.values[tag] = int(data)
        else:
            self.process_data(data)


class DisplayWithPeripherals(object):

    """
    The DisplayWithPeripherals tracks and updates sensor values for an ESP32 with peripherals

    :returns: returns nothing
    """

    def __init__(self):
        self.port = '/dev/cu.usbserial-023E564D'  # esp32
        self.baudrate = 115200
        self.s = serial.Serial(self.port, self.baudrate)
        self.values = defaultdict(int)
        self.parser = TagParser(self)

    """
    Updates the current sensor values using json format

    :returns: returns nothing
    """

    def readJSON(self):
        sensor = str(self.s.readline(), 'ascii').strip()
        print(sensor)
        self.values = json.loads(sensor)

    """
    Updates the current sensor values using tagging format

    :returns: returns nothing
    """

    def update(self):
        sensor = str(self.s.readline(), 'ascii').strip()
        self.parser.feed(sensor)

    """
    Returns the current button value

    :returns: returns current button value
    """

    def get_button(self):
        return self.button_val

    """
    Returns the current potentiometer value

    :returns: returns current potentiometer value
    """

    def get_potentiometer(self):
        return self.potentiometer_val

    """
    Returns the current joystick values

    :returns: returns current joystick values
    """

    def get_joystick(self):
        return self.joystick_vals


if __name__ == "__main__":
    display = DisplayWithPeripherals()
    while(True):
        # display.update()
        display.readJSON()
        print(display.values)
        print()
