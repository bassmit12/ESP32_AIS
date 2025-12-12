# ESP32 AIS Receiver - SignalK Gateway

Receives AIS vessel data via ESP-NOW and forwards to SignalK server via TCP.

## Configuration

### Method 1: Edit config.h (Simple)
Edit `include/config.h` and modify:
```cpp
#define WIFI_SSID "YourNetworkName"
#define WIFI_PASSWORD "YourPassword"
#define SIGNALK_SERVER "192.168.1.100"
#define SIGNALK_PORT 10110
```

### Method 2: Use Build Flags (Recommended for Production)
Edit `platformio.ini` and uncomment/modify the build_flags:
```ini
build_flags = 
  -DWIFI_SSID=\"YourSSID\"
  -DWIFI_PASSWORD=\"YourPassword\"
  -DSIGNALK_SERVER=\"192.168.1.100\"
  -DSIGNALK_PORT=10110
```

### Method 3: Environment Variables (CI/CD)
Set environment variables and PlatformIO will use them:
```bash
export WIFI_SSID="YourNetworkName"
export WIFI_PASSWORD="YourPassword"
export SIGNALK_SERVER="192.168.1.100"
export SIGNALK_PORT=10110
```

Then in `platformio.ini`:
```ini
build_flags = 
  -DWIFI_SSID=\"${sysenv.WIFI_SSID}\"
  -DWIFI_PASSWORD=\"${sysenv.WIFI_PASSWORD}\"
  -DSIGNALK_SERVER=\"${sysenv.SIGNALK_SERVER}\"
  -DSIGNALK_PORT=${sysenv.SIGNALK_PORT}
```

## SignalK Server Setup
The receiver connects to your SignalK server via TCP on port 10110 (default). Make sure:
1. SignalK server is running and accessible
2. TCP input is enabled on the specified port
3. The ESP32 is on the same network as the SignalK server

## Build and Upload
```bash
pio run --target upload
pio device monitor
```

## MAC Address
On first boot, the receiver will print its MAC address. Copy this to the sender's `config.h` to establish the ESP-NOW connection.
