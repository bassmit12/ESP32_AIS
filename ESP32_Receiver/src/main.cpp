#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "ESPNowReceiver.h"
#include "SignalKClient.h"
#include "DisplayReceiver.h"

ESPNowReceiver* receiver = nullptr;
SignalKClient* signalk = nullptr;

bool wifiConnected = false;
bool signalkConnected = false;
unsigned long lastStatusPrint = 0;

void onAISDataReceived(const AISData& data) {
    Serial.println("\n===============================================");
    Serial.println("    📡 ESP-NOW TRANSMISSION RECEIVED 📡");
    Serial.println("===============================================\n");
    
    DisplayReceiver::printAISData(data);
    
    if (signalk && signalk->isConnected()) {
        if (signalk->sendAISData(data)) {
            DisplayReceiver::printSignalKForward(200);
        } else {
            DisplayReceiver::printSignalKForwardFailed();
            signalkConnected = false;
        }
    } else {
        Serial.println("  ⚠ Skipping SignalK send - not connected\n");
    }
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
    DisplayReceiver::printWiFiStatus(connected, WiFi.localIP(), WiFi.RSSI(), WiFi.channel());
    wifiConnected = connected;
    return connected;
}

bool initESPNow() {
    receiver = new ESPNowReceiver();
    receiver->setDataCallback(onAISDataReceived);
    
    bool initialized = receiver->begin(ESPNOW_CHANNEL);
    
    uint8_t mac[6];
    WiFi.macAddress(mac);
    DisplayReceiver::printESPNowStatus(initialized, mac, ESPNOW_CHANNEL);
    
    return initialized;
}

void initSignalK() {
    signalk = new SignalKClient(SIGNALK_SERVER, SIGNALK_PORT);
    DisplayReceiver::printSignalKConfig(SIGNALK_SERVER, SIGNALK_PORT);
    
    if (signalk->connect()) {
        DisplayReceiver::printSignalKConnected();
        signalkConnected = true;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    DisplayReceiver::printHeader();
    
    initWiFi();
    
    if (!initESPNow()) {
        DisplayReceiver::printError("ESP-NOW initialization failed!");
        return;
    }
    
    initSignalK();
    
    DisplayReceiver::printConnectionStatus(wifiConnected, signalkConnected, true);
    
    Serial.println("🟢 SYSTEM READY - Waiting for AIS data...\n");
    Serial.println("===============================================\n");
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        if (wifiConnected) {
            Serial.println("┌─────────────────────────────────────┐");
            Serial.println("│ ✗ WiFi connection LOST!             │");
            Serial.println("└─────────────────────────────────────┘\n");
            wifiConnected = false;
        }
        
        static unsigned long lastWifiCheck = 0;
        if (millis() - lastWifiCheck > WIFI_RECONNECT_INTERVAL_MS) {
            lastWifiCheck = millis();
            Serial.println("→ Attempting WiFi reconnection...");
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        }
    } else {
        if (!wifiConnected) {
            Serial.println("┌─────────────────────────────────────┐");
            Serial.println("│ ✓ WiFi RECONNECTED!                 │");
            Serial.print  ("│ IP: ");
            Serial.println(WiFi.localIP());
            Serial.println("└─────────────────────────────────────┘\n");
            wifiConnected = true;
        }
    }
    
    if (signalk) {
        if (!signalk->isConnected() && signalkConnected) {
            DisplayReceiver::printSignalKDisconnected();
            signalkConnected = false;
        }
        
        signalk->reconnect();
        
        if (signalk->isConnected() && !signalkConnected) {
            DisplayReceiver::printSignalKConnected();
            signalkConnected = true;
        }
    }
    
    if (millis() - lastStatusPrint > STATUS_PRINT_INTERVAL_MS) {
        lastStatusPrint = millis();
        DisplayReceiver::printConnectionStatus(wifiConnected, signalkConnected, true);
    }
    
    delay(100);
}