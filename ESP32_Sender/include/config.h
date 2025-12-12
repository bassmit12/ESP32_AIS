#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

// Receiver ESP32 MAC Address
// Replace with your receiver's MAC address
#define RECEIVER_MAC_0 0xCC
#define RECEIVER_MAC_1 0x7B
#define RECEIVER_MAC_2 0x5C
#define RECEIVER_MAC_3 0x35
#define RECEIVER_MAC_4 0xE1
#define RECEIVER_MAC_5 0x1C

// Transmission Settings
#define SEND_INTERVAL_MS 3000

// Connection Settings
#define WIFI_TIMEOUT_SEC 20

#endif
