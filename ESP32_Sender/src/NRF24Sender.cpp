#include "NRF24Sender.h"
#include "config.h"

NRF24Sender::NRF24Sender(uint8_t cePin, uint8_t csnPin) 
    : _cePin(cePin), _csnPin(csnPin), _initialized(false) {
    _radio = new RF24(_cePin, _csnPin);
}

bool NRF24Sender::begin(uint8_t channel) {
    if (!_radio->begin()) {
        return false;
    }
    
    _radio->setPALevel(RF24_PA_HIGH);
    _radio->setDataRate(RF24_250KBPS);
    _radio->setChannel(channel);
    _radio->setRetries(15, 15);
    _radio->enableDynamicPayloads();
    _radio->setAutoAck(true);
    
    _radio->openWritingPipe(NRF24_TX_PIPE);
    _radio->openReadingPipe(1, NRF24_RX_PIPE);
    
    _radio->stopListening();
    
    _initialized = true;
    return true;
}

bool NRF24Sender::sendData(const AISData& data) {
    if (!_initialized) {
        return false;
    }
    
    bool result = _radio->write(&data, sizeof(AISData));
    return result;
}

bool NRF24Sender::isConnected() {
    return _initialized && _radio->isChipConnected();
}
