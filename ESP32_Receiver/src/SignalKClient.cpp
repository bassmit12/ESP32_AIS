#include "SignalKClient.h"

SignalKClient::SignalKClient(const char* server, uint16_t port) 
    : _server(server), _port(port), _lastReconnectAttempt(0), 
      _reconnectInterval(5000), _wasConnected(false) {
}

bool SignalKClient::connect() {
    if (_client.connected()) {
        if (!_wasConnected) {
            _wasConnected = true;
        }
        return true;
    }
    
    unsigned long currentTime = millis();
    if (currentTime - _lastReconnectAttempt < _reconnectInterval) {
        return false;
    }
    
    _lastReconnectAttempt = currentTime;
    _wasConnected = false;
    
    return _client.connect(_server, _port);
}

bool SignalKClient::isConnected() {
    return _client.connected();
}

void SignalKClient::reconnect() {
    if (!_client.connected()) {
        connect();
    }
}

bool SignalKClient::sendAISData(const AISData& data) {
    if (!_client.connected()) {
        return false;
    }
    
    char sentence[300];
    createAIVDMSentence(data, sentence, sizeof(sentence));
    
    size_t written = _client.print(sentence);
    
    if (written == 0) {
        _client.stop();
        _wasConnected = false;
        return false;
    }
    
    return true;
}

void SignalKClient::createAIVDMSentence(const AISData& data, char* sentence, size_t maxLen) {
    // Create AIS Message Type 1, 2, or 3 (Position Report with SOTDMA)
    // Using Message Type 1 for simplicity
    
    uint8_t payload[168] = {0}; // Bit array for AIS message (168 bits max for Type 1)
    
    // Message Type: 1 (6 bits)
    setBits(payload, 0, 6, 1);
    
    // Repeat Indicator: 0 (6-8, 2 bits)
    setBits(payload, 6, 2, 0);
    
    // MMSI: (8-38, 30 bits)
    setBits(payload, 8, 30, data.mmsi);
    
    // Navigation Status: (38-42, 4 bits)
    setBits(payload, 38, 4, data.navStatus);
    
    // Rate of Turn (ROT): 0 = not turning (42-50, 8 bits)
    setBits(payload, 42, 8, 128); // 128 = no turn information
    
    // Speed Over Ground: in 0.1 knot steps (50-60, 10 bits)
    uint16_t sog_encoded = (uint16_t)(data.sog * 10.0);
    if (sog_encoded > 1022) sog_encoded = 1022;
    setBits(payload, 50, 10, sog_encoded);
    
    // Position Accuracy: 0 = low (60-61, 1 bit)
    setBits(payload, 60, 1, 0);
    
    // Longitude: in 1/10000 minute (61-89, 28 bits)
    int32_t lon_encoded = (int32_t)(data.longitude * 600000.0);
    setBits(payload, 61, 28, lon_encoded & 0x0FFFFFFF);
    
    // Latitude: in 1/10000 minute (89-116, 27 bits)
    int32_t lat_encoded = (int32_t)(data.latitude * 600000.0);
    setBits(payload, 89, 27, lat_encoded & 0x07FFFFFF);
    
    // Course Over Ground: in 0.1 degree (116-128, 12 bits)
    uint16_t cog_encoded = (uint16_t)(data.cog * 10.0);
    if (cog_encoded >= 3600) cog_encoded = 3600;
    setBits(payload, 116, 12, cog_encoded);
    
    // True Heading: in degrees (128-137, 9 bits)
    uint16_t heading = data.heading;
    if (heading > 359) heading = 511; // 511 = not available
    setBits(payload, 128, 9, heading);
    
    // Time Stamp: seconds of UTC (137-143, 6 bits)
    setBits(payload, 137, 6, 60); // 60 = not available
    
    // Maneuver Indicator: 0 = not available (143-145, 2 bits)
    setBits(payload, 143, 2, 0);
    
    // Spare: 0 (145-148, 3 bits)
    setBits(payload, 145, 3, 0);
    
    // RAIM flag: 0 (148-149, 1 bit)
    setBits(payload, 148, 1, 0);
    
    // Radio status: 0 (149-168, 19 bits)
    setBits(payload, 149, 19, 0);
    
    // Convert to 6-bit ASCII armored format
    char armored[30];
    bitsToArmor(payload, 168, armored);
    
    // Build AIVDM sentence: !AIVDM,1,1,,A,<payload>,0*XX
    char baseSentence[256];
    snprintf(baseSentence, sizeof(baseSentence),
        "!AIVDM,1,1,,A,%s,0*", armored);
    
    uint8_t checksum = calculateChecksum(baseSentence);
    snprintf(sentence, maxLen, "%s%02X\r\n", baseSentence, checksum);
}

void SignalKClient::setBits(uint8_t* payload, int start, int len, uint32_t value) {
    for (int i = 0; i < len; i++) {
        int bitPos = start + i;
        int bytePos = bitPos / 8;
        int bitInByte = 7 - (bitPos % 8);
        
        if (value & (1 << (len - 1 - i))) {
            payload[bytePos] |= (1 << bitInByte);
        }
    }
}

void SignalKClient::bitsToArmor(uint8_t* payload, int numBits, char* result) {
    int numChars = (numBits + 5) / 6; // Round up to nearest 6 bits
    
    for (int i = 0; i < numChars; i++) {
        uint8_t sixBits = 0;
        
        // Extract 6 bits
        for (int j = 0; j < 6; j++) {
            int bitPos = i * 6 + j;
            if (bitPos < numBits) {
                int bytePos = bitPos / 8;
                int bitInByte = 7 - (bitPos % 8);
                
                if (payload[bytePos] & (1 << bitInByte)) {
                    sixBits |= (1 << (5 - j));
                }
            }
        }
        
        // Convert to ASCII armor (add 48, then add 8 if >= 88)
        if (sixBits < 40) {
            result[i] = sixBits + 48;
        } else {
            result[i] = sixBits + 56;
        }
    }
    
    result[numChars] = '\0';
}

void SignalKClient::latToNMEA(float lat, char* result, char* ns) {
    *ns = (lat >= 0) ? 'N' : 'S';
    float absLat = fabs(lat);
    int degrees = (int)absLat;
    float minutes = (absLat - degrees) * 60.0;
    sprintf(result, "%02d%07.4f", degrees, minutes);
}

void SignalKClient::lonToNMEA(float lon, char* result, char* ew) {
    *ew = (lon >= 0) ? 'E' : 'W';
    float absLon = fabs(lon);
    int degrees = (int)absLon;
    float minutes = (absLon - degrees) * 60.0;
    sprintf(result, "%03d%07.4f", degrees, minutes);
}

uint8_t SignalKClient::calculateChecksum(const char* sentence) {
    uint8_t checksum = 0;
    for (int i = 1; sentence[i] != '*' && sentence[i] != '\0'; i++) {
        checksum ^= sentence[i];
    }
    return checksum;
}
