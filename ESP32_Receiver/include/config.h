#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

// SignalK Server Configuration
#ifndef SIGNALK_SERVER
#define SIGNALK_SERVER "192.168.178.98"
#endif

#ifndef SIGNALK_PORT
#define SIGNALK_PORT 10110
#endif

// Connection Settings
#define WIFI_TIMEOUT_SEC 20
#define SIGNALK_RECONNECT_INTERVAL_MS 5000
#define STATUS_PRINT_INTERVAL_MS 30000
#define WIFI_RECONNECT_INTERVAL_MS 10000

// ESP-NOW Settings
#define ESPNOW_CHANNEL 1

#endif
