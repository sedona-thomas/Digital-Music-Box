#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    This code will read a UDP message from an ESP32

    @Author: Sedona Thomas
    @Date: 03/5/2022
    @Links: https://github.com/sedona-thomas/TTGO-Interactive-Device
"""

import socket


class UdpController(object):

    def __init__(self):
        # use ifconfig -a to find this IP. If your pi is the first and only device connected to the ESP32,
        # should be the correct IP by default on raspberry pi
        self.LOCAL_UDP_IP = "192.168.1.2"
        self.SHARED_UDP_PORT = 4210
        self.sock = socket.socket(
            socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
        self.sock.bind((self.LOCAL_UDP_IP, self.SHARED_UDP_PORT))

    def run(self):
        while True:
            data, addr = self.sock.recvfrom(2048)
            print(data)


if __name__ == "__main__":
    udp = UdpController()
    udp.run()
