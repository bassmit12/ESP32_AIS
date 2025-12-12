#ifndef NRF24_RECEIVER_H
#define NRF24_RECEIVER_H

#include <Arduino.h>
#include <RF24.h>
#include "AISData.h"

typedef void (*AISDataCallback)(const AISData& data);

class NRF24Receiver {
public:
    NRF24Receiver(uint8_t cePin, uint8_t csnPin);
    bool begin(uint8_t channel);
    void setDataCallback(AISDataCallback callback);
    void update();
    bool isConnected();
    
private:
    RF24* _radio;
    uint8_t _cePin;
    uint8_t _csnPin;
    bool _initialized;
    AISDataCallback _callback;
};

#endif
