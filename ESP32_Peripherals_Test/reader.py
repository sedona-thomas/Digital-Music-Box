#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''





    Setup:
        button = {"+": "3V", "-" : "12"}
        potentiometer = {"in": "3V", "out": "13", "ground": "G"}
        joystick = {"ground": "G", "5V": "5V", "VRx": "27", "VRy": "26", "SW": "25"}
'''

import serial
import re


class PeripheralTagParser(object):

    def __init__(self, device):
        self.tags = {"data": self.process_data,
                     "button": self.process_button,
                     "potentiometer": self.process_potentiometer,
                     "joystick": self.process_joystick,
                     "VRx": self.process_VRx,
                     "VRy": self.process_VRy,
                     "SW": self.process_SW}
        self.device = device

    def feed(self, text):
        print("Text:\n", text, "\n")
        remaining = text
        while remaining:
            tag, data, remaining = self.parse(remaining)
            if tag:
                self.process_tag(tag, data)

    def parse(self, text):
        text = text.strip()
        tag, open_tag, close_tag = extract_current_tag(self, text)
        data = text[open_tag[1]: close_tag[0]]
        remianing = text[close_tag[0]:]
        return tag, data, remaining

    def extract_current_tag(self, text):
        open_tag = re.search("<\s*\w*\s*>", text)
        tag = re.search("\w*", open_tag)
        close_tag = re.search("<\s*" + tag + "*\s*>", text)
        return tag, open_tag.span(), close_tag.span() if open_tag and close_tag else "", -1, -1

    def process_tag(self, tag, data):
        self.tags[tag](data)

    def process_data(self, data):
        self.parse(data)
        self.process_tag(tag, data)

    def process_button(self, data):
        if data:
            self.device.button_val = int(data)

    def process_potentiometer(self, data):
        if data:
            self.device.potentiometer_val = int(data)

    def process_joystick(self, data):
        pass

    def process_VRx(self, data):
        pass

    def process_VRy(self, data):
        pass

    def process_SW(self, data):
        pass


class DisplayWithPeripherals(object):

    def __init__(self):
        self.port = '/dev/cu.usbserial-023E564D'  # esp32
        self.baudrate = 115200
        self.s = serial.Serial(self.port, self.baudrate)
        self.button_val = 0
        self.potentiometer_val = 0
        self.joystick_vals = {"VRx": 0, "VRy": 0, "SW": 0}
        self.parser = PeripheralTagParser(self)

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


'''
class PeripheralTagParser(object):

    def __init__(self, device):
        self.tags = {"data": self.process_data,
                     "button": self.process_button,
                     "potentiometer": self.process_potentiometer,
                     "joystick": self.process_joystick,
                     "VRx": self.process_VRx,
                     "VRy": self.process_VRy,
                     "SW": self.process_SW}
        self.device = device

    def feed(self, text):
        print("Text:\n", text, "\n")
        remaining = text
        while remaining:
            tag, data, remaining = self.parse(remaining)
            if tag:
                self.process_tag(tag, data)

    def parse(self, text):
        text = text.strip()
        if "<" in text:
            open_tag, close_tag = self.extract_current_tag(text)
            tag_o = text[open_tag: text[open_tag].find(">")].strip()
            tag_c = text[close_tag: text[close_tag].find(">")].strip()
            if (tag_o == tag_c):
                data = text[text[open_tag].find(">") + 1: close_tag].strip()
                remaining = text[close_tag:]
                return tag_o, data, remaining
            else:
                print("Open tag doesn't match close tag for", tag, close_tag)
                return "", "", ""
        elif text:
            print("Information not caught by relevant method, text:", text)
            return "", "", ""

    def process_tag(self, tag, data):
        self.tags[tag](data)

    def process_data(self, data):
        self.parse(data)
        self.process_tag(tag, data)

    def process_button(self, data):
        if data:
            self.device.button_val = int(data)

    def process_potentiometer(self, data):
        if data:
            self.device.potentiometer_val = int(data)

    def process_joystick(self, data):
        pass

    def process_VRx(self, data):
        pass

    def process_VRy(self, data):
        pass

    def process_SW(self, data):
        pass

    def extract_current_tag(self, text):
        open_tag = text.find("<")
        tag = text[text.find("<") + 1: text.find(">")].strip()
        close_tag = -1
        current_text = text[text.find(">"):]
        while close_tag == -1:
            next_close_tag_open = current_text.rfind("</")
            next_tag = current_text.rfind(tag)
            if next_close_tag < next_tag:
                next_close_tag_close = current_text.rfind(">")
                close_tag = text.find(
                    current_text[next_close_tag_open: next_close_tag_open + 1])
        return open_tag, close_tag


from html.parser import HTMLParser
class SerialPeripheralParser(HTMLParser):

    def handle_starttag(self, tag, attrs):
        print("Encountered a start tag:", tag)

    def handle_endtag(self, tag):
        print("Encountered an end tag :", tag)

    def handle_data(self, data):
        print("Encountered some data  :", data)
'''
