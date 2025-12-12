#ifndef AISDATA_H
#define AISDATA_H

#include <Arduino.h>

struct AISData {
    uint32_t mmsi;           // Maritime Mobile Service Identity (unique ship ID)
    float latitude;          // Latitude in decimal degrees (-90 to 90)
    float longitude;         // Longitude in decimal degrees (-180 to 180)
    float sog;               // Speed Over Ground in knots
    float cog;               // Course Over Ground in degrees (0-360)
    uint16_t heading;        // True heading in degrees (0-359, 511 = not available)
    uint8_t navStatus;       // Navigation status (0=underway, 1=anchored, 5=moored, etc.)
    char vesselName[21];     // Vessel name (max 20 characters + null terminator)
};

// Navigation Status Constants
#define NAV_STATUS_UNDERWAY_ENGINE 0
#define NAV_STATUS_ANCHORED 1
#define NAV_STATUS_NOT_UNDER_COMMAND 2
#define NAV_STATUS_RESTRICTED_MANOEUVRABILITY 3
#define NAV_STATUS_CONSTRAINED_BY_DRAUGHT 4
#define NAV_STATUS_MOORED 5
#define NAV_STATUS_AGROUND 6
#define NAV_STATUS_FISHING 7
#define NAV_STATUS_UNDERWAY_SAILING 8

#endif
