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

// nRF24L01 Configuration
#ifndef NRF24_CE_PIN
#define NRF24_CE_PIN 4
#endif

#ifndef NRF24_CSN_PIN
#define NRF24_CSN_PIN 5
#endif

#ifndef NRF24_CHANNEL
#define NRF24_CHANNEL 76
#endif

// Pipe addresses for nRF24L01
const uint8_t NRF24_TX_PIPE[6] = "00001";
const uint8_t NRF24_RX_PIPE[6] = "00002";

// Connection Settings
#define WIFI_TIMEOUT_SEC 20
#define SIGNALK_RECONNECT_INTERVAL_MS 5000
#define STATUS_PRINT_INTERVAL_MS 30000
#define WIFI_RECONNECT_INTERVAL_MS 10000

#endif
