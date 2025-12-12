#include "Display.h"

void Display::printHeader() {
    Serial.println("\n\n");
    Serial.println("███████████████████████████████████████████████");
    Serial.println("█                                             █");
    Serial.println("█      ESP32 AIS SENDER (nRF24L01)           █");
    Serial.println("█                                             █");
    Serial.println("███████████████████████████████████████████████");
    Serial.println();
}

void Display::printWiFiStatus(bool connected, const IPAddress& ip, int channel) {
    Serial.println("┌─────────── WIFI SETUP ──────────────┐");
    if (connected) {
        Serial.println("│ ✓ WiFi CONNECTION SUCCESSFUL!");
        Serial.print("│ IP: ");
        Serial.println(ip);
        Serial.print("│ Channel: ");
        Serial.println(channel);
    } else {
        Serial.println("│ ✗ WiFi CONNECTION FAILED!");
        Serial.println("│ ⚠ Continuing anyway...");
    }
    Serial.println("└─────────────────────────────────────┘\n");
}

void Display::printESPNowStatus(bool initialized, const uint8_t* mac, int channel) {
    Serial.println("┌───────── nRF24L01 SETUP ────────────┐");
    Serial.print("│ ESP32 MAC: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    if (initialized) {
        Serial.println("│ ✓ nRF24L01 initialized");
        Serial.printf("│ ✓ Radio channel set to %d\n", channel);
    } else {
        Serial.println("│ ✗ ERROR: nRF24L01 init failed!");
    }
    Serial.println("└─────────────────────────────────────┘\n");
}

void Display::printVesselDatabase(int count) {
    Serial.println("┌──────── VESSEL DATABASE ────────────┐");
    Serial.printf("│ %d sample vessels loaded:           │\n", count);
    Serial.println("│  1. NORDIC FIGHTER (Cargo)          │");
    Serial.println("│  2. OCEAN HARVEST (Fishing)         │");
    Serial.println("│  3. SEABIRD (Anchored)              │");
    Serial.println("└─────────────────────────────────────┘\n");
    Serial.println("🟢 SYSTEM READY - Transmitting every 3 seconds\n");
    Serial.println("═════════════════════════════════════════════\n");
}

void Display::printTransmission(int count, int vesselIndex, const AISData& data) {
    Serial.println("┌─ TRANSMITTING AIS DATA ─────────────┐");
    Serial.printf("│ #%d Vessel %d/3: %s\n", count, vesselIndex + 1, data.vesselName);
    Serial.printf("│ MMSI: %lu\n", data.mmsi);
    Serial.printf("│ Position: %.4f, %.4f\n", data.latitude, data.longitude);
    Serial.printf("│ Speed: %.1f kts | Course: %.1f°\n", data.sog, data.cog);
    Serial.println("│ Sending via nRF24L01...");
}

void Display::printTransmissionResult(bool success) {
    Serial.print("  ");
    Serial.println(success ? "✓ TRANSMISSION SUCCESSFUL" : "✗ TRANSMISSION FAILED!");
    if (!success) {
        Serial.println("  ⚠ Check receiver is powered on");
        Serial.println("  ⚠ Check MAC address is correct");
    }
    Serial.println("─────────────────────────────────────────\n");
}

void Display::printError(const char* message) {
    Serial.print("✗ ERROR: ");
    Serial.println(message);
}

const char* Display::getNavStatusString(uint8_t status) {
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
