#include "NRF24Receiver.h"
#include "config.h"

NRF24Receiver::NRF24Receiver(uint8_t cePin, uint8_t csnPin) 
    : _cePin(cePin), _csnPin(csnPin), _initialized(false), _callback(nullptr) {
    _radio = new RF24(_cePin, _csnPin);
}

bool NRF24Receiver::begin(uint8_t channel) {
    if (!_radio->begin()) {
        return false;
    }
    
    _radio->setPALevel(RF24_PA_HIGH);
    _radio->setDataRate(RF24_250KBPS);
    _radio->setChannel(channel);
    _radio->enableDynamicPayloads();
    _radio->setAutoAck(true);
    
    _radio->openWritingPipe(NRF24_RX_PIPE);
    _radio->openReadingPipe(1, NRF24_TX_PIPE);
    
    _radio->startListening();
    
    _initialized = true;
    return true;
}

void NRF24Receiver::setDataCallback(AISDataCallback callback) {
    _callback = callback;
}

void NRF24Receiver::update() {
    if (!_initialized || !_callback) {
        return;
    }
    
    if (_radio->available()) {
        AISData data;
        _radio->read(&data, sizeof(AISData));
        _callback(data);
    }
}

bool NRF24Receiver::isConnected() {
    return _initialized && _radio->isChipConnected();
}
