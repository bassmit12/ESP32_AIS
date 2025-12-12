#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "ESPNowSender.h"
#include "Display.h"
#include "VesselData.h"

// Global objects
ESPNowSender* sender = nullptr;
VesselData* vesselData = nullptr;

unsigned long lastSendTime = 0;
int transmissionCount = 0;
bool lastTransmissionSuccess = false;

// Callback for transmission status
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  lastTransmissionSuccess = (status == ESP_NOW_SEND_SUCCESS);
  Display::printTransmissionResult(lastTransmissionSuccess);
}

bool initWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < WIFI_TIMEOUT_SEC) {
    delay(500);
    timeout++;
  }
  
  bool connected = (WiFi.status() == WL_CONNECTED);
  Display::printWiFiStatus(connected, WiFi.localIP(), WiFi.channel());
  return connected;
}

bool initESPNow() {
  uint8_t receiverMac[] = {
    RECEIVER_MAC_0, RECEIVER_MAC_1, RECEIVER_MAC_2,
    RECEIVER_MAC_3, RECEIVER_MAC_4, RECEIVER_MAC_5
  };
  
  sender = new ESPNowSender(receiverMac);
  sender->setCallback(onDataSent);
  
  bool initialized = sender->begin();
  
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Display::printESPNowStatus(initialized, mac);
  
  return initialized;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Display::printHeader();
  
  // Initialize WiFi
  initWiFi();
  
  // Initialize ESP-NOW
  if (!initESPNow()) {
    Display::printError("ESP-NOW initialization failed!");
    return;
  }
  
  // Initialize vessel data
  vesselData = new VesselData(3);
  vesselData->initialize();
  Display::printVesselDatabase(vesselData->getVesselCount());
}

void loop() {
  if (!sender || !vesselData) {
    return;
  }
  
  unsigned long currentTime = millis();
  
  if (currentTime - lastSendTime >= SEND_INTERVAL_MS) {
    lastSendTime = currentTime;
    transmissionCount++;
    
    AISData* vessel = vesselData->getCurrentVessel();
    
    Display::printTransmission(transmissionCount, vesselData->getCurrentIndex(), *vessel);
    
    if (!sender->sendData(*vessel)) {
      Display::printError("esp_now_send() failed");
    }
    
    vesselData->moveToNext();
  }
}
