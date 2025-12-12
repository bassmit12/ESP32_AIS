#include "ESPNowReceiver.h"
#include <esp_wifi.h>

AISDataCallback ESPNowReceiver::_dataCallback = nullptr;

ESPNowReceiver::ESPNowReceiver() {
}

bool ESPNowReceiver::begin(int channel) {
    if (esp_now_init() != ESP_OK) {
        return false;
    }
    
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_now_register_recv_cb(onDataReceive);
    
    return true;
}

void ESPNowReceiver::setDataCallback(AISDataCallback callback) {
    _dataCallback = callback;
}

void ESPNowReceiver::onDataReceive(const uint8_t *mac, const uint8_t *data, int len) {
    if (len != sizeof(AISData)) {
        return;
    }
    
    if (_dataCallback) {
        AISData aisData;
        memcpy(&aisData, data, sizeof(AISData));
        _dataCallback(aisData);
    }
}
