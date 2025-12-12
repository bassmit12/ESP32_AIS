#include "ESPNowSender.h"

ESPNowSender::ESPNowSender(const uint8_t* macAddress) : _initialized(false) {
    memcpy(_peerAddress, macAddress, 6);
}

bool ESPNowSender::begin() {
    if (esp_now_init() != ESP_OK) {
        return false;
    }
    
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, _peerAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        return false;
    }
    
    _initialized = true;
    return true;
}

bool ESPNowSender::sendData(const AISData& data) {
    if (!_initialized) {
        return false;
    }
    
    esp_err_t result = esp_now_send(_peerAddress, (uint8_t*)&data, sizeof(AISData));
    return (result == ESP_OK);
}

void ESPNowSender::setCallback(esp_now_send_cb_t callback) {
    esp_now_register_send_cb(callback);
}
