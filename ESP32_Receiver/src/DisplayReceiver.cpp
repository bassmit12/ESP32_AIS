#include "DisplayReceiver.h"

void DisplayReceiver::printHeader() {
    Serial.println("\n\n");
    Serial.println("███████████████████████████████████████████████");
    Serial.println("█                                             █");
    Serial.println("█   ESP32 AIS RECEIVER - SIGNALK GATEWAY     █");
    Serial.println("█                                             █");
    Serial.println("███████████████████████████████████████████████");
    Serial.println();
}

void DisplayReceiver::printWiFiStatus(bool connected, const IPAddress& ip, int rssi, int channel) {
    Serial.println("┌─────────── WIFI SETUP ──────────────┐");
    if (connected) {
        Serial.println("│ ✓ WiFi CONNECTION SUCCESSFUL!");
        Serial.print("│ IP: ");
        Serial.println(ip);
        Serial.print("│ Signal: ");
        Serial.print(rssi);
        Serial.println(" dBm");
        Serial.print("│ WiFi Channel: ");
        Serial.println(channel);
    } else {
        Serial.println("│ ✗ WiFi CONNECTION FAILED!");
        Serial.println("│ ⚠ Will continue trying...");
    }
    Serial.println("└─────────────────────────────────────┘\n");
}

void DisplayReceiver::printESPNowStatus(bool initialized, const uint8_t* mac, int channel) {
    Serial.println("┌───────── ESP-NOW SETUP ─────────────┐");
    Serial.print("│ MAC: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    if (initialized) {
        Serial.println("│ ✓ ESP-NOW initialized");
        Serial.printf("│ ✓ ESP-NOW channel set to %d\n", channel);
        Serial.println("│ ✓ Listening for AIS data...");
    } else {
        Serial.println("│ ✗ ERROR: Initialization failed!");
    }
    Serial.println("└─────────────────────────────────────┘\n");
}

void DisplayReceiver::printSignalKConfig(const char* server, uint16_t port) {
    Serial.println("┌──────── SIGNALK CONFIG ─────────────┐");
    Serial.print("│ Server: ");
    Serial.print(server);
    Serial.print(":");
    Serial.println(port);
    Serial.println("│ Connecting...");
    Serial.println("└─────────────────────────────────────┘\n");
}

void DisplayReceiver::printSignalKConnected() {
    Serial.println("┌────────────────────────────────────────┐");
    Serial.println("│ ✓ SignalK TCP CONNECTED                │");
    Serial.println("└────────────────────────────────────────┘\n");
}

void DisplayReceiver::printSignalKDisconnected() {
    Serial.println("┌────────────────────────────────────────┐");
    Serial.println("│ ✗ SignalK TCP DISCONNECTED             │");
    Serial.println("└────────────────────────────────────────┘\n");
}

void DisplayReceiver::printConnectionStatus(bool wifi, bool signalk, bool espnow) {
    Serial.println("\n┌──────────── CONNECTION STATUS ────────────┐");
    
    Serial.print("│ WiFi:    ");
    Serial.println(wifi ? "✓ CONNECTED                  " : "✗ DISCONNECTED                  ");
    
    Serial.print("│ SignalK: ");
    Serial.println(signalk ? "✓ CONNECTED                  " : "✗ DISCONNECTED                  ");
    
    Serial.print("│ ESP-NOW: ");
    Serial.println(espnow ? "✓ ACTIVE (listening)          " : "✗ INACTIVE                        ");
    
    Serial.println("└────────────────────────────────────────┘\n");
}

void DisplayReceiver::printAISData(const AISData& data) {
    Serial.println("╔═══════════════════════════════════════════════╗");
    Serial.println("║      🚢 AIS VESSEL DATA RECEIVED 🚢          ║");
    Serial.println("╠═══════════════════════════════════════════════╣");
    Serial.print("║ Vessel Name:  ");
    Serial.println(data.vesselName);
    Serial.print("║ MMSI:         ");
    Serial.println(data.mmsi);
    Serial.print("║ Position:     ");
    Serial.print(data.latitude, 6);
    Serial.print("° N, ");
    Serial.print(data.longitude, 6);
    Serial.println("° E");
    Serial.print("║ Speed (SOG):  ");
    Serial.print(data.sog, 1);
    Serial.println(" knots");
    Serial.print("║ Course (COG): ");
    Serial.print(data.cog, 1);
    Serial.println("°");
    Serial.print("║ Heading:      ");
    if (data.heading == 511) {
        Serial.println("Not Available");
    } else {
        Serial.print(data.heading);
        Serial.println("°");
    }
    Serial.print("║ Nav Status:   ");
    Serial.println(getNavStatusString(data.navStatus));
    Serial.println("╚═══════════════════════════════════════════════╝");
}

void DisplayReceiver::printSignalKForward(size_t bytes) {
    Serial.println("  ┌─ SignalK Forward (NMEA0183) ──────┐");
    Serial.print("  │ ✓ Sent ");
    Serial.print(bytes);
    Serial.println(" bytes (NMEA)          │");
    Serial.println("  │ Format: NMEA0183/AIS               │");
    Serial.println("  └────────────────────────────────────┘\n");
}

void DisplayReceiver::printSignalKForwardFailed() {
    Serial.println("  ┌─ SignalK Forward (NMEA0183) ──────┐");
    Serial.println("  │ ✗ FAILED                           │");
    Serial.println("  └────────────────────────────────────┘\n");
}

void DisplayReceiver::printError(const char* message) {
    Serial.print("⚠ ERROR: ");
    Serial.println(message);
}

const char* DisplayReceiver::getNavStatusString(uint8_t status) {
    switch(status) {
        case NAV_STATUS_UNDERWAY_ENGINE: return "Underway (Engine)";
        case NAV_STATUS_ANCHORED: return "Anchored";
        case NAV_STATUS_NOT_UNDER_COMMAND: return "Not Under Command";
        case NAV_STATUS_RESTRICTED_MANOEUVRABILITY: return "Restricted Manoeuvrability";
        case NAV_STATUS_CONSTRAINED_BY_DRAUGHT: return "Constrained by Draught";
        case NAV_STATUS_MOORED: return "Moored";
        case NAV_STATUS_AGROUND: return "Aground";
        case NAV_STATUS_FISHING: return "Fishing";
        case NAV_STATUS_UNDERWAY_SAILING: return "Underway (Sailing)";
        default: return "Unknown";
    }
}
