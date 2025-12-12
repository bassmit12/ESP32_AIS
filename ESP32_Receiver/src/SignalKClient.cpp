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
    
    // Create a simple but complete NMEA sentence with vessel identification
    // Using a custom format that includes MMSI for proper vessel tracking
    char sentence[300];
    
    char latStr[16], lonStr[16];
    char ns, ew;
    latToNMEA(data.latitude, latStr, &ns);
    lonToNMEA(data.longitude, lonStr, &ew);
    
    // Create a proprietary NMEA sentence with full vessel info
    char baseSentence[256];
    snprintf(baseSentence, sizeof(baseSentence),
        "$PNAIS,MMSI=%lu,LAT=%s,%c,LON=%s,%c,SOG=%.1f,COG=%.1f,HDG=%u,NAME=%s,STATUS=%u*",
        data.mmsi, latStr, ns, lonStr, ew, data.sog, data.cog, 
        data.heading, data.vesselName, data.navStatus
    );
    
    uint8_t checksum = calculateChecksum(baseSentence);
    snprintf(sentence, sizeof(sentence), "%s%02X\r\n", baseSentence, checksum);
    
    size_t written = _client.print(sentence);
    
    if (written == 0) {
        _client.stop();
        _wasConnected = false;
        return false;
    }
    
    return true;
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
