#ifndef VESSEL_DATA_H
#define VESSEL_DATA_H

#include <Arduino.h>
#include "AISData.h"

// Waypoint structure for route following
struct Waypoint {
    float lat;
    float lon;
};

class VesselData {
public:
    VesselData(int maxVessels = 2);
    ~VesselData();
    
    void initialize();
    void updatePositions(unsigned long deltaTime);
    AISData* getCurrentVessel();
    void moveToNext();
    int getVesselCount() const { return _vesselCount; }
    int getCurrentIndex() const { return _currentIndex; }
    
private:
    AISData* _vessels;
    int* _waypointIndices;
    int _vesselCount;
    int _currentIndex;
    
    void updateVesselPosition(int vesselIndex, unsigned long deltaTime);
    void createVessel(int index, uint32_t mmsi, const char* name, 
                     float lat, float lon, float sog, float cog, 
                     uint16_t heading, uint8_t status);
};

#endif
