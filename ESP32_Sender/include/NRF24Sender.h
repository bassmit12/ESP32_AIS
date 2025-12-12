#ifndef NRF24_SENDER_H
#define NRF24_SENDER_H

#include <Arduino.h>
#include <RF24.h>
#include "AISData.h"

class NRF24Sender {
public:
    NRF24Sender(uint8_t cePin, uint8_t csnPin);
    bool begin(uint8_t channel);
    bool sendData(const AISData& data);
    bool isConnected();
    
private:
    RF24* _radio;
    uint8_t _cePin;
    uint8_t _csnPin;
    bool _initialized;
};

#endif
