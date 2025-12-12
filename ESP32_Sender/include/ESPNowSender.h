#ifndef ESPNOW_SENDER_H
#define ESPNOW_SENDER_H

#include <Arduino.h>
#include <esp_now.h>
#include "AISData.h"

class ESPNowSender {
public:
    ESPNowSender(const uint8_t* macAddress);
    bool begin();
    bool sendData(const AISData& data);
    void setCallback(esp_now_send_cb_t callback);
    
private:
    uint8_t _peerAddress[6];
    bool _initialized;
};

#endif
