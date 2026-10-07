# ESP32 Wi-Fi Deauthentication Detector

A standalone wireless reconnaissance instrument built using the ESP32 framework. It sniffs 802.11 management frames in promiscuous mode to flag active network interruptions.

## Features
- **Asynchronous Spectrum Scanning:** Dynamic channel hopping loop (Channels 1-13).
- **Attack Intensity Telemetry:** Live Packets-Per-Second (PPS) calculation.
- **Visual Analytics:** Real-time vertical histogram displaying rolling RSSI signal footprints.
- **Zero Audio Profile:** Visual alerts via an I2C SSD1306/SH1106 OLED screen instead of intrusive buzzers.

## Hardware Components
- XIAO ESP32C6 - Amazon - ₹1,690 - 18$
- SSD1306 128x64 I2C OLED Display - Amazon - ₹649 - 6.71$
- Connecting Jumper Wires - Free cuz I already have them

## Software Requirements
- VS Code + PlatformIO IDE Extension OR Arduino IDE with ESP32 board plugin
- Adafruit SSD1306, GFX, and BusIO Libraries
