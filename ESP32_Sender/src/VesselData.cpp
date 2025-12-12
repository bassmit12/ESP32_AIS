#include "VesselData.h"

VesselData::VesselData(int maxVessels) : _vesselCount(maxVessels), _currentIndex(0) {
    _vessels = new AISData[_vesselCount];
}

VesselData::~VesselData() {
    delete[] _vessels;
}

void VesselData::initialize() {
    // Vessel 1: Cargo ship near Rotterdam
    createVessel(0, 244620000, "NORDIC FIGHTER", 
                51.9225, 4.4792, 12.5, 285.0, 287, NAV_STATUS_UNDERWAY_ENGINE);
    
    // Vessel 2: Fishing vessel
    createVessel(1, 219012345, "OCEAN HARVEST", 
                51.8945, 4.3210, 3.2, 145.0, 148, NAV_STATUS_FISHING);
    
    // Vessel 3: Anchored vessel
    createVessel(2, 211234567, "SEABIRD", 
                51.9556, 4.5123, 0.1, 0.0, 312, NAV_STATUS_ANCHORED);
}

AISData* VesselData::getCurrentVessel() {
    return &_vessels[_currentIndex];
}

void VesselData::moveToNext() {
    _currentIndex = (_currentIndex + 1) % _vesselCount;
}

void VesselData::createVessel(int index, uint32_t mmsi, const char* name, 
                             float lat, float lon, float sog, float cog, 
                             uint16_t heading, uint8_t status) {
    _vessels[index].mmsi = mmsi;
    _vessels[index].latitude = lat;
    _vessels[index].longitude = lon;
    _vessels[index].sog = sog;
    _vessels[index].cog = cog;
    _vessels[index].heading = heading;
    _vessels[index].navStatus = status;
    strncpy(_vessels[index].vesselName, name, 20);
    _vessels[index].vesselName[20] = '\0';
}
