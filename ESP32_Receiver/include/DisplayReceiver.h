#ifndef DISPLAY_RECEIVER_H
#define DISPLAY_RECEIVER_H

#include <Arduino.h>
#include "AISData.h"

class DisplayReceiver {
public:
    static void printHeader();
    static void printWiFiStatus(bool connected, const IPAddress& ip, int rssi, int channel);
    static void printESPNowStatus(bool initialized, const uint8_t* mac, int channel);
    static void printSignalKConfig(const char* server, uint16_t port);
    static void printSignalKConnected();
    static void printSignalKDisconnected();
    static void printConnectionStatus(bool wifi, bool signalk, bool espnow);
    static void printAISData(const AISData& data);
    static void printSignalKForward(size_t bytes);
    static void printSignalKForwardFailed();
    static void printError(const char* message);
    
private:
    static const char* getNavStatusString(uint8_t status);
};

#endif
