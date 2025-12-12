#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "NRF24Sender.h"
#include "Display.h"
#include "VesselData.h"

NRF24Sender* sender = nullptr;
VesselData* vesselData = nullptr;

unsigned long lastSendTime = 0;
int transmissionCount = 0;

bool initWiFi() {
  WiFi.mode(WIFI_STA);
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

bool initNRF24() {
  sender = new NRF24Sender(NRF24_CE_PIN, NRF24_CSN_PIN);
  
  bool initialized = sender->begin(NRF24_CHANNEL);
  
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Display::printESPNowStatus(initialized, mac, NRF24_CHANNEL);
  
  return initialized;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Display::printHeader();
  
  initWiFi();
  
  if (!initNRF24()) {
    Display::printError("nRF24L01 initialization failed!");
    return;
  }
  
  vesselData = new VesselData(2);
  vesselData->initialize();
  Display::printVesselDatabase(vesselData->getVesselCount());
}

void loop() {
  if (!sender || !vesselData) {
    return;
  }
  
  if (!sender->isConnected()) {
    Display::printError("nRF24L01 disconnected!");
    delay(1000);
    return;
  }
  
  static unsigned long lastUpdateTime = 0;
  unsigned long currentTime = millis();
  
  // Update vessel position every second
  if (currentTime - lastUpdateTime >= 1000) {
    unsigned long deltaTime = currentTime - lastUpdateTime;
    vesselData->updatePositions(deltaTime);
    lastUpdateTime = currentTime;
  }
  
  // Send AIS data at configured interval
  if (currentTime - lastSendTime >= SEND_INTERVAL_MS) {
    lastSendTime = currentTime;
    transmissionCount++;
    
    AISData* vessel = vesselData->getCurrentVessel();
    
    Display::printTransmission(transmissionCount, vesselData->getCurrentIndex(), *vessel);
    
    bool success = sender->sendData(*vessel);
    Display::printTransmissionResult(success);
    
    if (!success) {
      Display::printError("Transmission failed - check nRF24L01 connection");
    }
    
    vesselData->moveToNext();
  }
}
