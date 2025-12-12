#ifndef SIGNALK_CLIENT_H
#define SIGNALK_CLIENT_H

#include <Arduino.h>
#include <WiFiClient.h>
#include "AISData.h"

class SignalKClient {
public:
    SignalKClient(const char* server, uint16_t port);
    bool connect();
    bool isConnected();
    bool sendAISData(const AISData& data);
    void reconnect();
    
private:
    const char* _server;
    uint16_t _port;
    WiFiClient _client;
    unsigned long _lastReconnectAttempt;
    unsigned long _reconnectInterval;
    bool _wasConnected;
    
    void createAIVDMSentence(const AISData& data, char* sentence, size_t maxLen);
    void setBits(uint8_t* payload, int start, int len, uint32_t value);
    void bitsToArmor(uint8_t* payload, int numBits, char* result);
    void latToNMEA(float lat, char* result, char* ns);
    void lonToNMEA(float lon, char* result, char* ew);
    uint8_t calculateChecksum(const char* sentence);
};

#endif
