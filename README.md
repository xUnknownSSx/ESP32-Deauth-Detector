# ESP32 Wi-Fi Deauthentication Detector

A standalone wireless reconnaissance instrument built using the ESP32 framework. It sniffs 802.11 management frames in promiscuous mode to flag active network interruptions.

## Features
- **Asynchronous Spectrum Scanning:** Dynamic channel hopping loop (Channels 1-13).
- **Attack Intensity Telemetry:** Live Packets-Per-Second (PPS) calculation.
- **Visual Analytics:** Real-time vertical histogram displaying rolling RSSI signal footprints.
- **Zero Audio Profile:** Visual alerts via an I2C SSD1306/SH1106 OLED screen instead of intrusive buzzers.

## Hardware Components
- XIAO ESP32C6 OR ESP32 Devkit (not recommended)
- SSD1306 128x64 I2C OLED Display 
- Connecting Jumper Wires

**This project doesnt need a PCB or a 3D printed case, as its meant to be as simple as possible!**

## Software/Frimware
Check main.cpp for the firmware for this project. 

## Pin Layout

1. ESP32 DevKit
| OLED Module Pin | Target ESP32 DevKit GPIO | Description / Functional Assignment |
| **VCC** | **5V / VIN** | Primary Power Rail |
| **GND** | **GND** | Common Ground Line Connection |
| **SCL** (Clock) | **GPIO 22** | Default Hardware I2C Clock Bus Register |
| **SDA** (Data) | **GPIO 21** | Default Hardware I2C Data Bus Register |

2. XIAO ESP32 C6
| OLED Module Pin | Target XIAO ESP32-C6 Pin | Description / Functional Assignment |
| **VCC** | **5V** or **3V3** | Direct USB/Regulated Power Input Rail |
| **GND** | **GND** | Common Ground Line Connection |
| **SCL** (Clock) | **D5 (GPIO 7)** | Next-Gen Hardware I2C Clock Bus Register |
| **SDA** (Data) | **D4 (GPIO 6)** | Next-Gen Hardware I2C Data Bus Register |

## Software Requirements
- VS Code + PlatformIO IDE Extension OR Arduino IDE with ESP32 board plugin
- Adafruit SSD1306, GFX, and BusIO Libraries

## Why I Thought About Making This??
I have seen many cybersecurity experts trying to get information on when a particular deauth frame is flooded over the network on which BSSID, MAC, ESSID. This device solves it all! Me myself as a cybersecurity enthusiast, i love to make such projects which help solve simple problems for cybersecurity experts and hackers. 

## BOM
- XIAO ESP32C6 - Amazon - ₹1,690 - 18$
- SSD1306 128x64 I2C OLED Display - Amazon - ₹649 - 6.71$
- Connecting Jumper Wires - Free cuz I already have them
