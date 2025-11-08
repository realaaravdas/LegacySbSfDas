# ESP32 PWM Solutions for Minibots

## Problem Summary
The Arduino IDE is not recognizing ESP32 LEDC functions (`ledcSetup`, `ledcAttachPin`, `ledcWrite`) for your ESP32 WROOM board.

## Solution Options (Choose ONE)

### Option 1: Use the Simple Version (Recommended for Arduino IDE)

File: `minibots_simple.ino`

**Key Changes:**
- Uses standard `analogWrite()` and fallback digital output
- Maintains original motor pins (GPIO 16 and 19)
- Added PWM testing code at startup
- Maximum compatibility across different ESP32 Arduino cores

**To Use:**
1. Rename `minibots_simple.ino` to `minibots.ino`
2. Use existing GPIO 16 (left) and GPIO 19 (right) connections
3. Compile normally in Arduino IDE

### Option 1B: Use the Enhanced LEDC Version

File: `minibots_ledc_working.ino` 

**Key Changes:**
- Uses direct ESP32 LEDC API (bypasses Arduino wrappers)
- Maintains original motor pins (GPIO 16 and 19)
- Extensive error reporting and debugging
- Fallback to analogWrite if LEDC fails

**To Use:**
1. Rename `minibots_ledc_working.ino` to `minibots.ino`
2. Use existing GPIO 16 (left) and GPIO 19 (right) connections
3. Update your ESP32 Arduino core to latest version if needed

### Option 2: Use PlatformIO (Recommended Professional Solution)

**Installation:**
1. Install VS Code
2. Install PlatformIO Extension
3. Open the minibots folder as a PlatformIO project
4. Run: `pio run --target upload`

**PlatformIO Advantages:**
- Proper ESP32 toolchain configuration
- Automatic library dependency management
- Multi-board support (ESP32, NodeMCU-32S, etc.)
- Integrated debugging and serial monitor

### Option 3: Fix Arduino IDE ESP32 Core Installation

**Step-by-Step:**
1. **Arduino IDE → File → Preferences**
2. Add to "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **Tools → Board → Boards Manager**
4. Search "esp32" - install "esp32 by Espressif Systems"
5. **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
6. Restart Arduino IDE completely
7. Try compiling the original `minibots.ino`

### Option 4: Updated LEDC Functions (Advanced Version)

The current `minibots.ino` now includes:
- Direct ESP32 LEDC API calls that bypass Arduino wrapper
- Compatibility layer for different ESP32 cores
- Fallback modes for non-ESP32 platforms

## Motor Pin Configuration

### Simple Version (minibots_simple.ino):
```
Left Motor:  GPIO 16
Right Motor: GPIO 19
```

### LEDC Working Version (minibots_ledc_working.ino):
```
Left Motor:  GPIO 16
Right Motor: GPIO 19
```

### Original Version (minibots.ino):
```
Left Motor:  GPIO 16
Right Motor: GPIO 19
```

## Wiring Instructions

### For Simple Version & LEDC Working Version:
- Connect left motor controller to GPIO 16
- Connect right motor controller to GPIO 19
- Ensure motor driver has separate power supply
- Connect ESP32 GND to motor driver GND

### For Original Version:
- Connect left motor controller to GPIO 16
- Connect right motor controller to GPIO 19
- Ensure motor driver has separate power supply
- Connect ESP32 GND to motor driver GND

## Testing the Solution

### Regardless of which option you choose:

1. **Compile the code**
2. **Upload to ESP32**
3. **Open Serial Monitor** (115200 baud)
4. **Expected Output:**
   ```
   Connecting to WiFi...
   Connected to WiFi
   IP Address: 192.168.1.100
   Broadcast UDP server started on port 2367
   Data UDP server started on port 2380
   PWM pins initialized
   ```
5. **Test with DriverStation** - should see parsing and motor value output

## Troubleshooting

### If compilation still fails:
- Ensure ESP32 board manager is properly installed
- Try a different USB port
- Check that all files are in the same sketch folder
- Restart Arduino IDE completely

### If motors don't move:
- Verify wiring connections
- Check motor driver power supply
- Monitor serial output for data parsing
- Test with direct write: `analogWrite(5, 255);`

### If WiFi connection fails:
- Verify SSID and password are correct
- Check network signal strength
- Try moving closer to WiFi router

## PWM Technical Details

### ESP32 LEDC (Advanced Version):
- 16 independent channels
- Configurable frequency and resolution
- Hardware-based PWM generation
- 5000Hz frequency, 8-bit resolution (0-255)

### Arduino analogWrite (Simple Version):
- Software-emulated PWM on most pins
- Variable frequency (typically 500Hz-1kHz)
- 8-bit resolution (0-255)
- Good enough for basic motor control

## Performance Comparison

| Method | Precision | CPU Load | Compatibility |
|--------|-----------|----------|--------------|
| LEDC API | High | Low | ESP32 only |
| analogWrite | Medium | Medium | All boards |

For robot motor control, both methods provide adequate performance. The simple `analogWrite` approach is recommended for ease of use and maximum compatibility.
