#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial

port = '/dev/cu.usbserial-023E564D'  # esp32
baudrate = 115200

s = serial.Serial(port, baudrate)

while(True):
    print(get_sensor_values())

    # button = get_button()
    # potentiometer = get_potentiometer()
    # joystick = get_joystick()

    # print("current values:")
    # print(button)
    # print(potentiometer)
    # print(joystick)


def get_sensor_value():
    sensor = s.readline().strip()
    return str(sensor, 'ascii')


def get_button():
    button = {"-": 0}
    button_pins = {"-": 12}

    return button


def get_potentiometer():
    potentiometer = {"in": 0, "out": 0, "ground": 0}
    potentiometer_pins = {"out": 13}

    return potentiometer


def get_joystick():
    joystick = {"ground": 0, "5V": 0, "VRx": 0, "VRy": 0, "SW": 0}
    joystick_pins = {"VRx": 27, "VRy": 26, "SW": 25}

    return joystick

# setup
# button = {"+": "3V", "-" : "12"}
# potentiometer = {"in": "3V", "out": "13", "ground": "G"}
# joystick = {"ground": "G", "5V": "5V", "VRx": "27", "VRy": "26", "SW": "25"}
