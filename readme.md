# ESP8266 Web Server with Custom HTML Pages

A lightweight HTTP web server implementation for the ESP8266 microcontroller that serves custom HTML, CSS, and JavaScript files from the device's file system (SPIFFS). Control GPIO pins and manage your IoT devices through an interactive web interface.

## Features

- ✅ HTTP web server on port 80
- ✅ Serve custom HTML, CSS, and JavaScript files
- ✅ REST API endpoints for device control
- ✅ LED control (on/off/toggle)
- ✅ JSON status responses
- ✅ Real-time dashboard updates
- ✅ Interrupt-driven UART communication
- ✅ File system support (SPIFFS)

## Hardware Requirements

- **ESP8266** (NodeMCU, WeMos D1 Mini, or Generic ESP8266 Module)
- **USB Cable** (data cable, not charge-only)
- **LED** (optional, for testing) with resistor (~330Ω)
- **WiFi network** (2.4GHz)

## Software Requirements

- **Arduino IDE** (1.8.x or later)
- **ESP8266 Board Package** (via Arduino Board Manager)
- **SPIFFS Uploader Tool**

## Installation

### 1. Install ESP8266 Board Package

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Go to **Tools → Board Manager**
5. Search for "ESP8266" and install **"esp8266 by ESP8266 Community"**

### 2. Install SPIFFS Uploader Tool

1. Download from: https://github.com/earlephilhower/arduino-esp8266fs-plugin/releases
2. Extract the `ESP8266FS` folder
3. Copy to: `~/Arduino/tools/` (create `tools` directory if it doesn't exist)
4. Restart Arduino IDE
5. You should now see **Tools → ESP8266 SPIFFS Data Upload**

### 3. Board Configuration

In Arduino IDE:
- **Tools → Board** → Select "Generic ESP8266 Module" (or your specific board)
- **Tools → Port** → Select your COM port
- **Tools → Upload Speed** → 115200

## Project Structure

```
esp8266-webserver/
├── webserver/
│   ├── webserver.ino          # Main Arduino sketch
│   └── data/
│       ├── index.html         # Main web page
│       ├── style.css          # (Optional) Stylesheet
│       └── script.js          # (Optional) JavaScript
├── README.md
└── LICENSE
```

## Usage

### 1. Configure WiFi Credentials

Edit the `.ino` file and update:

```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```

### 2. Create Your Web Pages

In the `data/` folder, create your HTML files:

**data/index.html** (minimal example):
```html
<!DOCTYPE html>
<html>
<head>
    <title>ESP8266 Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
    <h1>ESP8266 Web Server</h1>
    <button onclick="fetch('/api/led/on')">LED ON</button>
    <button onclick="fetch('/api/led/off')">LED OFF</button>
    <button onclick="fetch('/api/led/toggle')">TOGGLE</button>
    <p>Status: <span id="status">Loading...</span></p>
    
    <script>
        function updateStatus() {
            fetch('/api/status')
                .then(r => r.json())
                .then(d => document.getElementById('status').innerText = d.led_state);
        }
        setInterval(updateStatus, 2000);
        updateStatus();
    </script>
</body>
</html>
```

### 3. Upload Sketch

1. Open `webserver.ino` in Arduino IDE
2. Click **Upload** (or Ctrl+U)
3. Wait for compilation and upload to complete

### 4. Upload Web Files

1. Make sure your `data/` folder contains your HTML/CSS/JS files
2. Go to **Tools → ESP8266 SPIFFS Data Upload**
3. Wait for "LittleFS Image Uploaded" message in Serial Monitor

### 5. Connect and Access

1. Open **Tools → Serial Monitor** (115200 baud)
2. You should see the IP address printed (e.g., `192.168.1.100`)
3. Open `http://192.168.1.100` in your browser
4. Your custom web page should load!

## API Endpoints

| Endpoint | Method | Response | Description |
|----------|--------|----------|-------------|
| `/` | GET | HTML | Serves `index.html` |
| `/api/led/on` | GET | JSON | Turn LED on |
| `/api/led/off` | GET | JSON | Turn LED off |
| `/api/led/toggle` | GET | JSON | Toggle LED state |
| `/api/status` | GET | JSON | Get current LED status |

### JSON Response Format

```json
{
  "led_state": "ON",
  "uptime": 12345
}
```

## Troubleshooting

### Port Not Showing in Arduino IDE

**Windows:**
- Install CH340 drivers from: https://github.com/juliagoda/CH341SER

**Linux (Zorin OS / Ubuntu):**
```bash
sudo apt update
sudo apt install -y linux-headers-$(uname -r) build-essential git
git clone https://github.com/juliagoda/CH341SER.git
cd CH341SER
sudo make install
sudo modprobe ch341
```

If `brltty` conflicts:
```bash
sudo systemctl mask brltty
sudo reboot
```

**macOS:**
- Install from: https://sparks.gogo.co.nz/ch340.html

### Files Not Uploading

1. Ensure `data/` folder exists in your sketch directory
2. Place files directly in `data/` folder (not subdirectories)
3. File names must be lowercase
4. Check Serial Monitor for list of uploaded files

### "404 - File Not Found"

- Verify files were uploaded: check Serial Monitor for "Files in SPIFFS:"
- Ensure file names are correct (case-sensitive)
- Try uploading SPIFFS again: **Tools → ESP8266 SPIFFS Data Upload**

### ESP8266 Keeps Disconnecting

- Try a different USB cable (some are charge-only)
- Try a different USB port on your computer
- Check if you're using a USB hub (may cause power issues)

## LED Setup (Optional)

If you want to test with a physical LED:

```
ESP8266 Pin D4 (GPIO2) ──┬── 330Ω Resistor ──┬── LED Anode
                         │                    │
                       GND ────────────────────── LED Cathode
```

To use a different pin, modify:
```cpp
const int LED_PIN = 2;  // Change this to your GPIO number
```

## Performance Tips

- **Minimize HTML file size** for faster loading
- **Use CSS instead of inline styles** for better organization
- **Compress images** before uploading to SPIFFS
- **Limit JSON responses** to essential data only
- SPIFFS has limited space (typically 64-256KB depending on board)

## File Size Limits

- **Total SPIFFS space**: 64-256KB (varies by board)
- **Individual file limit**: Up to available SPIFFS space
- **ESP8266 RAM**: ~80KB (consider this when building responses)

## References

- [ESP8266 Official Documentation](https://arduino-esp8266.readthedocs.io/)
- [ESP8266 WebServer Library](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/README.md)
- [SPIFFS File System](https://github.com/pellepl/spiffs)

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

For issues, questions, or suggestions, please open an issue on GitHub.

---

**Happy Coding!** 🚀