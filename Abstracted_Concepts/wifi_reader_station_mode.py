#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    This code will send an initial packet, then listen for packets from the ESP32

    @Author: Sedona Thomas
    @Date: 03/5/2022
    @Links: https://github.com/sedona-thomas/TTGO-Interactive-Device
"""

import socket


class UdpController(object):

    def __init__(self):
        # The IP that is printed in the serial monitor from the ESP32
        self.UDP_IP = "192.168.137.96"
        self.SHARED_UDP_PORT = 4210
        self.sock = socket.socket(
            socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
        self.sock.connect((self.UDP_IP, self.SHARED_UDP_PORT))
        self.setup()

    def setup(self):
        sock.send('Connecting to ESP32'.encode())

    def run(self):
        while True:
            data = sock.recv(2048)
            print(data)


if __name__ == "__main__":
    udp = UdpController()
    udp.run()
