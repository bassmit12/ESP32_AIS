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
    
    char sentence1[300];
    char sentence2[300];
    
    // Create NMEA sentences
    char latStr[16], lonStr[16];
    char ns, ew;
    latToNMEA(data.latitude, latStr, &ns);
    lonToNMEA(data.longitude, lonStr, &ew);
    
    char baseSentence[256];
    snprintf(baseSentence, sizeof(baseSentence),
        "$GPGGA,120000,%s,%c,%s,%c,1,08,0.9,%.1f,M,0.0,M,,*",
        latStr, ns, lonStr, ew, (float)data.sog
    );
    
    uint8_t checksum = calculateChecksum(baseSentence);
    snprintf(sentence1, sizeof(sentence1), "%s%02X\r\n", baseSentence, checksum);
    
    char vesselInfo[256];
    snprintf(vesselInfo, sizeof(vesselInfo),
        "$PNEPT,MMSI=%lu,NAME=%s,SOG=%.1f,COG=%.1f,HDG=%u,STATUS=%u*",
        data.mmsi, data.vesselName, data.sog, data.cog, data.heading, data.navStatus
    );
    
    checksum = calculateChecksum(vesselInfo);
    snprintf(sentence2, sizeof(sentence2), "%s%02X\r\n", vesselInfo, checksum);
    
    size_t written = _client.print(sentence1) + _client.print(sentence2);
    
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
