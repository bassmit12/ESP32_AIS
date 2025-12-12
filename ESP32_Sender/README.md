# ESP32 AIS Sender

Sends simulated AIS vessel data via ESP-NOW to the receiver.

## Configuration

### Method 1: Edit config.h (Simple)
Edit `include/config.h` and modify:
```cpp
#define WIFI_SSID "YourNetworkName"
#define WIFI_PASSWORD "YourPassword"
#define RECEIVER_MAC_0 0xCC  // Update with your receiver's MAC
// ... etc
```

### Method 2: Use Build Flags (Recommended for Production)
Edit `platformio.ini` and uncomment/modify the build_flags:
```ini
build_flags = 
  -DWIFI_SSID=\"YourSSID\"
  -DWIFI_PASSWORD=\"YourPassword\"
```

### Method 3: Environment Variables (CI/CD)
Set environment variables and PlatformIO will use them:
```bash
export WIFI_SSID="YourNetworkName"
export WIFI_PASSWORD="YourPassword"
```

Then in `platformio.ini`:
```ini
build_flags = 
  -DWIFI_SSID=\"${sysenv.WIFI_SSID}\"
  -DWIFI_PASSWORD=\"${sysenv.WIFI_PASSWORD}\"
```

## Receiver MAC Address
Find your receiver's MAC address by uploading the receiver code first. It will print the MAC address on startup. Then update the MAC address in `config.h`.

## Build and Upload
```bash
pio run --target upload
pio device monitor
```
