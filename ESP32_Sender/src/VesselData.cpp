#include "VesselData.h"

// Circular route waypoints (clockwise) - Vessel 1
const Waypoint circularRoute[] = {
    {51.45292, 5.47737},  // WP01-A
    {51.45195, 5.49522},  // WP02-B
    {51.44479, 5.50225},  // WP03-C
    {51.43323, 5.50328},  // WP04-D
    {51.42424, 5.48904},  // WP05-E
    {51.42702, 5.46485},  // WP06-F
    {51.43665, 5.45215},  // WP07-G
    {51.44778, 5.45129},  // WP08-H
    {51.45195, 5.46588}   // WP09-I
};

// Square route waypoints (clockwise) - Vessel 2
const Waypoint squareRoute[] = {
    {51.20176, 5.95629},  // WP01-A
    {51.20043, 5.96481},  // WP02-B
    {51.19563, 5.96314},  // WP03-C
    {51.19877, 5.95057}   // WP04-D
};

const int CIRCULAR_ROUTE_SIZE = sizeof(circularRoute) / sizeof(circularRoute[0]);
const int SQUARE_ROUTE_SIZE = sizeof(squareRoute) / sizeof(squareRoute[0]);

VesselData::VesselData(int maxVessels) : _vesselCount(maxVessels), _currentIndex(0) {
    _vessels = new AISData[_vesselCount];
    _waypointIndices = new int[_vesselCount];
    for (int i = 0; i < _vesselCount; i++) {
        _waypointIndices[i] = 0;
    }
}

VesselData::~VesselData() {
    delete[] _vessels;
    delete[] _waypointIndices;
}

// Calculate distance between two coordinates (Haversine formula)
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
    const float R = 6371000; // Earth radius in meters
    float dLat = (lat2 - lat1) * PI / 180.0;
    float dLon = (lon2 - lon1) * PI / 180.0;
    
    float a = sin(dLat/2) * sin(dLat/2) +
              cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) *
              sin(dLon/2) * sin(dLon/2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return R * c;
}

// Calculate bearing between two coordinates
float calculateBearing(float lat1, float lon1, float lat2, float lon2) {
    float dLon = (lon2 - lon1) * PI / 180.0;
    float y = sin(dLon) * cos(lat2 * PI / 180.0);
    float x = cos(lat1 * PI / 180.0) * sin(lat2 * PI / 180.0) -
              sin(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) * cos(dLon);
    float bearing = atan2(y, x) * 180.0 / PI;
    
    return fmod((bearing + 360.0), 360.0);
}

void VesselData::initialize() {
    // Vessel 1: Circular route patrol
    createVessel(0, 244123456, "ROUTE PATROL", 
                circularRoute[0].lat, circularRoute[0].lon, 8.5, 135.0, 135, NAV_STATUS_UNDERWAY_ENGINE);
    
    // Vessel 2: Square route patrol
    createVessel(1, 244789012, "SQUARE PATROL", 
                squareRoute[0].lat, squareRoute[0].lon, 7.0, 90.0, 90, NAV_STATUS_UNDERWAY_ENGINE);
}

void VesselData::updatePositions(unsigned long deltaTime) {
    // Update all vessels
    for (int i = 0; i < _vesselCount; i++) {
        updateVesselPosition(i, deltaTime);
    }
}

void VesselData::updateVesselPosition(int vesselIndex, unsigned long deltaTime) {
    AISData* vessel = &_vessels[vesselIndex];
    
    // Determine which route this vessel follows
    const Waypoint* route;
    int routeSize;
    
    if (vesselIndex == 0) {
        // Vessel 1: Circular route
        route = circularRoute;
        routeSize = CIRCULAR_ROUTE_SIZE;
    } else {
        // Vessel 2: Square route
        route = squareRoute;
        routeSize = SQUARE_ROUTE_SIZE;
    }
    
    // Speed in m/s (1 knot = 0.514444 m/s)
    float speedMS = vessel->sog * 0.514444;
    float distanceToMove = speedMS * (deltaTime / 1000.0);
    
    // Get current and next waypoint
    int currentWP = _waypointIndices[vesselIndex];
    int nextWP = (currentWP + 1) % routeSize;
    
    float nextLat = route[nextWP].lat;
    float nextLon = route[nextWP].lon;
    
    // Calculate distance to next waypoint
    float distToWaypoint = calculateDistance(vessel->latitude, vessel->longitude, nextLat, nextLon);
    
    // Check if we've reached the waypoint (within 10 meters)
    if (distToWaypoint < 10.0) {
        _waypointIndices[vesselIndex] = nextWP;
        currentWP = nextWP;
        nextWP = (currentWP + 1) % routeSize;
        nextLat = route[nextWP].lat;
        nextLon = route[nextWP].lon;
    }
    
    // Calculate bearing to next waypoint
    vessel->cog = calculateBearing(vessel->latitude, vessel->longitude, nextLat, nextLon);
    vessel->heading = (uint16_t)vessel->cog;
    
    // Move vessel towards next waypoint
    float bearing = vessel->cog * PI / 180.0;
    
    // Calculate new position
    const float R = 6371000; // Earth radius in meters
    float lat1 = vessel->latitude * PI / 180.0;
    float lon1 = vessel->longitude * PI / 180.0;
    
    float lat2 = asin(sin(lat1) * cos(distanceToMove / R) +
                      cos(lat1) * sin(distanceToMove / R) * cos(bearing));
    
    float lon2 = lon1 + atan2(sin(bearing) * sin(distanceToMove / R) * cos(lat1),
                              cos(distanceToMove / R) - sin(lat1) * sin(lat2));
    
    vessel->latitude = lat2 * 180.0 / PI;
    vessel->longitude = lon2 * 180.0 / PI;
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
