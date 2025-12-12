# Neptargos - ESP32 AIS System

A complete AIS (Automatic Identification System) data transmission and reception system using two ESP32 devices with ESP-NOW wireless protocol, forwarding to SignalK server.

## 🎯 What This Does

- **ESP32 Sender** broadcasts simulated AIS vessel data via ESP-NOW
- **ESP32 Receiver** receives the data and forwards it to a SignalK server via WiFi
- **SignalK Server** displays vessel positions and data in real-time

```
┌─────────────┐   ESP-NOW    ┌──────────────┐   WiFi/TCP   ┌──────────┐
│ ESP32       │─────────────>│ ESP32        │────────────>│ SignalK  │
│ Sender      │  (Wireless)  │ Receiver     │   (NMEA)    │ Server   │
└─────────────┘              └──────────────┘             └──────────┘
  Transmits AIS                 Receives &                 Web UI
  vessel data                   forwards data              Displays ships
```

## 📋 Requirements

### Hardware
- 2x ESP32 development boards (ESP32 Wroom)
- 2x USB cables for programming
- Computer with USB ports
- (Optional) Raspberry Pi or server for SignalK

### Software
- [PlatformIO](https://platformio.org/) (VS Code extension recommended)
- [Docker](https://www.docker.com/) (for SignalK server)
- Python 3.x (automatically installed with PlatformIO)

## 🚀 Quick Start Guide

### Step 1: Get the Receiver's MAC Address

1. **Upload the receiver code first:**
   ```bash
   cd ESP32_Receiver
   pio run --target upload --upload-port COM8
   ```
   
2. **Open serial monitor and note the MAC address:**
   ```bash
   pio device monitor --port COM8
   ```
   
3. **You'll see something like:**
   ```
   MAC: CC:7B:5C:35:E1:1C
   ```
   **Write this down!** You'll need it for the sender.

### Step 2: Configure the Sender

1. **Edit `ESP32_Sender/platformio.ini`:**
   ```ini
   build_flags = 
       -DWIFI_SSID=\"YourWiFiName\"
       -DWIFI_PASSWORD=\"YourWiFiPassword\"
       -DRECEIVER_MAC_0=0xCC
       -DRECEIVER_MAC_1=0x7B
       -DRECEIVER_MAC_2=0x5C
       -DRECEIVER_MAC_3=0x35
       -DRECEIVER_MAC_4=0xE1
       -DRECEIVER_MAC_5=0x1C
   ```
   Replace the MAC address with the one from Step 1.

2. **Upload to sender:**
   ```bash
   cd ESP32_Sender
   pio run --target upload --upload-port COM7
   ```

### Step 3: Configure the Receiver

1. **Edit `ESP32_Receiver/platformio.ini`:**
   ```ini
   build_flags = 
       -DWIFI_SSID=\"YourWiFiName\"
       -DWIFI_PASSWORD=\"YourWiFiPassword\"
       -DSIGNALK_SERVER=\"192.168.1.100\"  # Your SignalK server IP
       -DSIGNALK_PORT=10110
   ```

2. **Upload to receiver:**
   ```bash
   cd ESP32_Receiver
   pio run --target upload --upload-port COM8
   ```

### Step 4: Start SignalK Server

1. **Edit `docker-compose.yml` if needed** (uses default settings)

2. **Start the server:**
   ```bash
   docker compose up -d
   ```

3. **Open SignalK in browser:**
   ```
   http://localhost:3000
   ```
   Or use your Raspberry Pi's IP: `http://192.168.1.100:3000`

4. **First-time setup:**
   - Create admin account
   - Go to **Server → Data Connections**
   - Add **TCP** connection on port **10110**
   - Input type: **NMEA 0183**
   - Click **Apply** and **Restart**

### Step 5: Watch It Work!

Open serial monitors for both ESP32s:

**Sender Monitor:**
```bash
cd ESP32_Sender
pio device monitor --port COM7
```

**Receiver Monitor:**
```bash
cd ESP32_Receiver  
pio device monitor --port COM8
```

You should see:
- **Sender:** Transmitting vessel data every 3 seconds
- **Receiver:** Receiving data and forwarding to SignalK
- **SignalK:** Vessels appearing on the map!

## 🔧 Configuration Reference

### ESP32 Sender Configuration

Edit `ESP32_Sender/platformio.ini`:

| Variable | Description | Example |
|----------|-------------|---------|
| `WIFI_SSID` | Your WiFi network name | `"MyWiFi"` |
| `WIFI_PASSWORD` | Your WiFi password | `"MyPassword123"` |
| `RECEIVER_MAC_0` to `RECEIVER_MAC_5` | Receiver's MAC address | `0xCC, 0x7B, 0x5C...` |
| `SEND_INTERVAL_MS` | Time between transmissions (ms) | `3000` (3 seconds) |
| `ESPNOW_CHANNEL` | WiFi channel for ESP-NOW | `1` |

### ESP32 Receiver Configuration

Edit `ESP32_Receiver/platformio.ini`:

| Variable | Description | Example |
|----------|-------------|---------|
| `WIFI_SSID` | Your WiFi network name | `"MyWiFi"` |
| `WIFI_PASSWORD` | Your WiFi password | `"MyPassword123"` |
| `SIGNALK_SERVER` | SignalK server IP address | `"192.168.1.100"` |
| `SIGNALK_PORT` | SignalK TCP port | `10110` |
| `ESPNOW_CHANNEL` | WiFi channel (must match sender) | `1` |

### SignalK Server Configuration

Edit `docker-compose.yml`:

| Variable | Description | Default |
|----------|-------------|---------|
| `TZ` | Timezone | `Europe/Amsterdam` |
| Port `3000` | SignalK web interface | Required |
| Port `10110` | TCP input for AIS data | Required |

## 📁 Project Structure

```
Neptargos/
├── ESP32_Sender/          # Transmitter firmware
│   ├── include/
│   │   ├── AISData.h      # AIS data structures
│   │   ├── config.h       # Configuration header
│   │   ├── ESPNowSender.h # ESP-NOW module
│   │   ├── Display.h      # Display module
│   │   └── VesselData.h   # Vessel database
│   ├── src/
│   │   ├── main.cpp       # Main application
│   │   ├── ESPNowSender.cpp
│   │   ├── Display.cpp
│   │   └── VesselData.cpp
│   └── platformio.ini     # Build configuration
│
├── ESP32_Receiver/        # Receiver firmware
│   ├── include/
│   │   ├── AISData.h      # AIS data structures
│   │   ├── config.h       # Configuration header
│   │   ├── ESPNowReceiver.h    # ESP-NOW receive module
│   │   ├── SignalKClient.h     # SignalK TCP client
│   │   └── DisplayReceiver.h   # Display module
│   ├── src/
│   │   ├── main.cpp       # Main application
│   │   ├── ESPNowReceiver.cpp
│   │   ├── SignalKClient.cpp
│   │   └── DisplayReceiver.cpp
│   └── platformio.ini     # Build configuration
│
├── docker-compose.yml     # SignalK server setup
├── upload_sender.ps1      # Upload script for sender
├── upload_receiver.ps1    # Upload script for receiver
├── monitor_sender.ps1     # Monitor script for sender
└── monitor_receiver.ps1   # Monitor script for receiver
```

## 🛠️ Helper Scripts (Windows PowerShell)

### Upload and Monitor

**Sender (COM7):**
```powershell
.\upload_sender.ps1
```

**Receiver (COM8):**
```powershell
.\upload_receiver.ps1
```

### Monitor Only

**Sender:**
```powershell
.\monitor_sender.ps1
```

**Receiver:**
```powershell
.\monitor_receiver.ps1
```

## 🐛 Troubleshooting

### ESP32 Not Connecting to WiFi

**Problem:** WiFi connection failed
**Solution:**
1. Double-check SSID and password in `platformio.ini`
2. Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
3. Check WiFi signal strength

### Receiver Not Getting Data from Sender

**Problem:** No ESP-NOW transmissions received
**Solution:**
1. Verify MAC address in sender config matches receiver
2. Check both devices are on same ESP-NOW channel
3. Ensure devices are within range (~100m)
4. Try power cycling both devices

### SignalK Not Receiving Data

**Problem:** No vessels appearing in SignalK
**Solution:**
1. Check receiver serial monitor - is it forwarding data?
2. Verify SignalK server IP is correct
3. Check port 10110 is open: `telnet 192.168.1.100 10110`
4. Verify TCP connection is configured in SignalK

### Upload Fails

**Problem:** `Could not open COM7/COM8`
**Solution:**
1. Close any serial monitors
2. Check correct COM port in scripts/command
3. Press BOOT button on ESP32 during upload
4. Try different USB cable/port

### Build Errors

**Problem:** Compilation errors
**Solution:**
1. Clean build: `pio run --target clean`
2. Update PlatformIO: `pio upgrade`
3. Check platformio.ini syntax

## 📊 Sample Vessel Data

The sender transmits 3 sample vessels in rotation:

| Vessel | MMSI | Type | Location |
|--------|------|------|----------|
| NORDIC FIGHTER | 244620000 | Cargo | 51.9225°N, 4.4792°E |
| OCEAN HARVEST | 219012345 | Fishing | 51.8945°N, 4.3210°E |
| SEABIRD | 211234567 | Anchored | 51.9556°N, 4.5123°E |

## 🔐 Security Note

⚠️ **WiFi credentials are stored in `platformio.ini`**
- Don't commit this file with real passwords to public repos
- Use environment variables for production
- ESP-NOW is unencrypted by default

## 📚 Additional Resources

- [SignalK Documentation](https://signalk.org/documentation/)
- [ESP-NOW Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)
- [PlatformIO Docs](https://docs.platformio.org/)
- [AIS Protocol](https://en.wikipedia.org/wiki/Automatic_identification_system)

## 🤝 Contributing

Feel free to:
- Add more vessel types
- Implement real AIS decoding from radio
- Add OLED/TFT display support
- Add MQTT publishing
- Improve error handling

## 📝 License

MIT License - Use freely, attribution appreciated!

## 🎉 Success Indicators

When everything is working correctly, you'll see:

### Sender Serial Output:
```
ESP32 AIS SENDER (ESP-NOW)
✓ WiFi CONNECTION SUCCESSFUL!
✓ ESP-NOW initialized
🟢 SYSTEM READY - Transmitting every 3 seconds

#1 Vessel 1/3: NORDIC FIGHTER
MMSI: 244620000
Position: 51.9225, 4.4792
✓ TRANSMISSION SUCCESSFUL
```

### Receiver Serial Output:
```
ESP32 AIS RECEIVER - SIGNALK GATEWAY
✓ WiFi CONNECTION SUCCESSFUL!
✓ ESP-NOW initialized
✓ SignalK TCP CONNECTED

📡 ESP-NOW TRANSMISSION RECEIVED 📡
Vessel Name: NORDIC FIGHTER
MMSI: 244620000
✓ Sent 200 bytes (NMEA)
```

### SignalK Web UI:
- Navigate to `http://your-server:3000`
- Click on **Webapps → Map**
- See vessels appearing and moving!

---

**Happy Sailing! ⛵**

For questions or issues, check the troubleshooting section or open an issue.
