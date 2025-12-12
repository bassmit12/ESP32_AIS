#ifndef ESPNOW_RECEIVER_H
#define ESPNOW_RECEIVER_H

#include <Arduino.h>
#include <esp_now.h>
#include "AISData.h"

typedef void (*AISDataCallback)(const AISData& data);

class ESPNowReceiver {
public:
    ESPNowReceiver();
    bool begin(int channel = 1);
    void setDataCallback(AISDataCallback callback);
    
private:
    static AISDataCallback _dataCallback;
    static void onDataReceive(const uint8_t *mac, const uint8_t *data, int len);
};

#endif
