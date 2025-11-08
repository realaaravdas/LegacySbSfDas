# ESP32 Minibots Code Fixes

## Issues Fixed

### 1. Missing `abs()` function declaration
**Problem**: `abs()` function was not available in `transform.cpp`
**Solution**: Added `#include <Arduino.h>` to access Arduino's abs() function

### 2. ESP32 LEDC PWM function declarations
**Problem**: `ledcSetup` and `ledcAttachPin` functions not recognized
**Solution**: Added proper Arduino ESP32 includes to access LEDC PWM functions

### 3. String parsing functions
**Problem**: `strtok()` and `atoi()` availability
**Solution**: Added `<string.h>` and `<stdlib.h>` includes

## Files Modified

### `transform.cpp`
- Added `#include <Arduino.h>` at the top
- This provides access to Arduino's abs() function

### `minibots.ino` 
- Added essential Arduino and C library includes
- Added debugging output for data parsing
- Improved error handling with minimum data requirements (2 values vs 8)

### `platformio.ini` (new file)
- Created PlatformIO configuration for ESP32 development
- Ensures proper ESP32 framework and build settings

## Compilation Instructions

### Using Arduino IDE
1. Install ESP32 board support in Arduino IDE
2. Select Board: "ESP32 Dev Module"
3. Add all files to the sketch folder
4. Compile and upload

### Using PlatformIO (Recommended)
1. Install VS Code with PlatformIO extension
2. Open the minibots folder as a PlatformIO project
3. Run `pio run` to compile
4. Run `pio run --target upload` to upload

## Hardware Configuration

### Motor Pins
- Left Motor: GPIO 16
- Right Motor: GPIO 19

### PWM Settings
- Frequency: 5000 Hz
- Resolution: 8 bits (0-255)
- Channels: 0 (left), 1 (right)

### Network Settings
- WiFi Network: ROBOTWIFINET
- Password: robo8711$$W
- Broadcast Port: 2367
- Data Port: 2380

## Testing

After flashing the code:
1. Open Serial Monitor at 115200 baud
2. Verify WiFi connection
3. Check for "Parsing robot data..." messages when using DriverStation
4. Monitor motor values in serial output

## DriverStation Protocol

The robot expects data in format: `!ROB#value1:value2:...:value8`
- `value1`: Left joystick Y-axis (0-255)  
- `value2`: Right joystick X-axis (0-255)
- `value3-8`: Additional data (currently unused)

Response to broadcasts: `ROB:minibotTestingDas`

## Troubleshooting

### Compilation Errors
- Ensure ESP32 board support is installed
- Check that all files are in the same directory
- Verify Arduino IDE is set to correct board

### Runtime Issues
- Check WiFi credentials are correct
- Ensure motor pins are properly wired
- Monitor serial output for debugging information

### PWM Issues
- Ensure motor driver can handle 5kHz PWM
- Check GPIO pin assignment matches hardware
- Verify motor driver power supply

## Future Improvements

- Add OTA (Over-The-Air) update capability
- Implement motor speed feedback
- Add battery voltage monitoring
- Include emergency stop functionality
