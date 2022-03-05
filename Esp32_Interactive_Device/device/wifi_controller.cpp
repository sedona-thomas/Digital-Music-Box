#include "wifi_controller.h"

void setupWifi() {
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void send(std::string message) {
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf(message);
  Udp.endPacket();
  delay(WAIT);
}