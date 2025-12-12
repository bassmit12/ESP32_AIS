#ifndef VESSEL_DATA_H
#define VESSEL_DATA_H

#include <Arduino.h>
#include "AISData.h"

class VesselData {
public:
    VesselData(int maxVessels = 3);
    ~VesselData();
    
    void initialize();
    AISData* getCurrentVessel();
    void moveToNext();
    int getVesselCount() const { return _vesselCount; }
    int getCurrentIndex() const { return _currentIndex; }
    
private:
    AISData* _vessels;
    int _vesselCount;
    int _currentIndex;
    
    void createVessel(int index, uint32_t mmsi, const char* name, 
                     float lat, float lon, float sog, float cog, 
                     uint16_t heading, uint8_t status);
};

#endif
