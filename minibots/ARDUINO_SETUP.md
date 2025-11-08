# Arduino IDE ESP32 Setup Instructions

## Issue Resolution

The compilation errors with `ledcSetup` and `ledcAttachPin` are caused by the Arduino IDE not having the proper ESP32 board configuration or using an outdated ESP32 core.

## Solution A: Update ESP32 Board Support

1. **Open Arduino IDE**
2. **File → Preferences**
3. **Additional Boards Manager URLs**: Add this URL:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. **Tools → Board → Boards Manager**
5. Search for "ESP32"
6. Install "esp32 by Espressif Systems" (version 2.0+)
7. **Tools → Board → ESP32 Arduino → ESP32 Dev Module**

## Solution B: Update Board Configuration Settings

After installing ESP32 support, set these options:

- **Board**: "ESP32 Dev Module" or "NodeMCU-32S"
- **Upload Speed**: "921600"
- **CPU Frequency**: "240MHz (WiFi/BT)"
- **Flash Frequency**: "80MHz"
- **Flash Mode**: "QIO"
- **Flash Size**: "4MB (32Mb)"
- **Partition Scheme**: "Huge APP (3MB No OTA/1MB SPIFFS)"
- **Core Debug Level**: "Info"
- **Port**: Select your ESP32's COM port

## Solution C: Alternative PWM Implementation

If LEDC functions still don't work, the code includes fallback functions:

The `setupPWM()` and `writePWM()` functions provide compatibility with:
- ESP32 native LEDC functions (preferred)
- Standard Arduino `analogWrite()` (fallback)

## Testing the Fix

1. **Compile**: Verify → Compile/Check (Ctrl+R)
2. **Upload**: Sketch → Upload (Ctrl+U)
3. **Monitor**: Tools → Serial Monitor (set to 115200 baud)

## Expected Output

```
Connecting to WiFi...
Connected to WiFi
IP Address: 192.168.1.100
Broadcast UDP server started on port 2367
Data UDP server started on port 2380
Parsing robot data...
Value 0: 128
Value 1: 128
Left Y: 128, Right X: 128
Left Motor: 127, Right Motor: 127
```

## Trouleshooting

### If compilation still fails:
1. Restart Arduino IDE completely
2. Ensure ESP32 board is correctly selected
3. Check that all files are in the same sketch folder
4. Try a different USB port

### If LEDC functions not found:
1. Verify ESP32 core version is 2.0 or later
2. Use PlatformIO (recommended for professional development):
   ```bash
   pio run --target upload
   ```

### Alternative Development Setup

**PlatformIO (Recommended)**:
1. Install VS Code
2. Install PlatformIO extension
3. Open minibots folder
4. Run: `pio run --target upload`

**Arduino CLI**:
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 minibots
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 minibots
```

## Notes on PWM Implementation

The ESP32 uses LEDC (LED Control) peripheral for PWM:
- **Frequency**: 5000 Hz (good for motor control)
- **Resolution**: 8 bits (0-255 duty cycle)
- **Channels**: Independent signals for each motor

If using the fallback `analogWrite()`, the frequency and resolution may vary by board.
