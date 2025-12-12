#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "AISData.h"

class Display {
public:
    static void printHeader();
    static void printWiFiStatus(bool connected, const IPAddress& ip, int channel);
    static void printESPNowStatus(bool initialized, const uint8_t* mac, int channel);
    static void printVesselDatabase(int count);
    static void printTransmission(int count, int vesselIndex, const AISData& data);
    static void printTransmissionResult(bool success);
    static void printError(const char* message);
    
private:
    static const char* getNavStatusString(uint8_t status);
};

#endif
