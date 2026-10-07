#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp_wifi.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Hardware Timing Configurations
uint8_t current_channel = 1;
unsigned long lastChannelHopTime = 0;
#define HOP_INTERVAL 150 

// Core Sniffer Logging Variables
unsigned long attackCounter = 0;
bool attackDetected = false;
unsigned long lastAttackTime = 0;

// Dynamic Metrics for Live Text Feed Layer
int currentRSSI = -100;
uint8_t attackChannel = 1;

// Graph Array Structures
#define GRAPH_WIDTH 50
#define GRAPH_LEFT_OFFSET 74
int rssiHistory[GRAPH_WIDTH];
int graphIndex = 0;
unsigned long lastGraphUpdateTime = 0;

void sniffer_callback(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return; 

  wifi_promiscuous_pkt_t* packet = (wifi_promiscuous_pkt_t*)buf;
  uint8_t* payload = packet->payload;
  
  // Dereference payload safely to inspect header bits cleanly
  uint8_t frame_type = (payload[0] & 0x0C) >> 2;
  uint8_t frame_subtype = (payload[0] & 0xF0) >> 4;

  if (frame_type == 0 && (frame_subtype == 0x0C || frame_subtype == 0x0A)) {
    attackCounter++;
    attackDetected = true;
    lastAttackTime = millis();
    currentRSSI = packet->rx_ctrl.rssi;
    attackChannel = current_channel;
  }
}

void pushToGraph(int rssiVal) {
  rssiHistory[graphIndex] = rssiVal;
  graphIndex = (graphIndex + 1) % GRAPH_WIDTH;
}

void renderLayout() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // --- LEFT HALF: INTERFACE EMOTICON CHASSIS ---
  if (attackDetected) {
    display.setTextSize(1);
    display.setCursor(0, 4);
    display.print("ATTACK!");
    
    display.setTextSize(2);
    display.setCursor(5, 24);
    display.print("X_X");
  } else {
    display.setTextSize(1);
    display.setCursor(0, 4);
    display.print("Scanning");
    
    display.setTextSize(2);
    display.setCursor(5, 24);
    display.print("^_^");
  }

  // --- RIGHT HALF: ROLLING GRAPH MATRIX ---
  display.drawFastVLine(GRAPH_LEFT_OFFSET - 4, 0, 42, SSD1306_WHITE); // Column boundary divider
  
  for (int i = 0; i < GRAPH_WIDTH; i++) {
    int dataPointIndex = (graphIndex + i) % GRAPH_WIDTH;
    int pointRssi = rssiHistory[dataPointIndex];
    
    int constrainedRssi = constrain(pointRssi, -100, -30);
    int barHeight = map(constrainedRssi, -100, -30, 0, 36);
    
    display.drawFastVLine(GRAPH_LEFT_OFFSET + i, 39 - barHeight, barHeight, SSD1306_WHITE);
  }

  // --- LOWER REGION: REAL-TIME TELEMETRY DATA PANEL ---
  display.drawFastHLine(0, 44, 128, SSD1306_WHITE); // Horizontal break line
  display.setTextSize(1);
  display.setCursor(0, 52);
  
  if (attackDetected) {
    display.print("Dth:");
    display.print(attackCounter);
    display.print(" Ch:");
    display.print(attackChannel);
    display.print(" RSSI:");
    display.print(currentRSSI);
  } else {
    display.print("Logs caught: ");
    display.print(attackCounter);
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for(;;); 
  }
  
  for(int i = 0; i < GRAPH_WIDTH; i++) {
    rssiHistory[i] = -100;
  }

  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&sniffer_callback);
  esp_wifi_set_channel(current_channel, WIFI_SECOND_CHAN_NONE);
  
  renderLayout();
}

void loop() {
  unsigned long currentTime = millis();

  // Shift graph columns dynamically every 500ms
  if (currentTime - lastGraphUpdateTime >= 500) {
    lastGraphUpdateTime = currentTime;
    if (attackDetected) {
      pushToGraph(currentRSSI);
    } else {
      pushToGraph(-100);
    }
  }

  if (attackDetected) {
    renderLayout();
    
    // Hold alert layout for 3.5 seconds before releasing channel latch
    if (currentTime - lastAttackTime > 3500) {
      attackDetected = false;
      currentRSSI = -100;
      lastChannelHopTime = currentTime; 
      renderLayout();
    }
  } 
  else {
    if (currentTime - lastChannelHopTime >= HOP_INTERVAL) {
      lastChannelHopTime = currentTime;
      
      current_channel++;
      if (current_channel > 13) {
        current_channel = 1;
      }
      
      esp_wifi_set_channel(current_channel, WIFI_SECOND_CHAN_NONE);
      renderLayout();
    }
  }
  delay(1); 
}
