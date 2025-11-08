# DriverStation Launcher Scripts

This directory contains automated launcher scripts for the SoccerBot Software DriverStation application.

## Quick Start

### Option 1: PowerShell (Recommended)
```powershell
.\Run-DriverStation.ps1 -Build
```

### Option 2: Batch File (Simple)
```cmd
Run-DriverStation.bat
```

### Option 3: Manual Launch
1. Install Qt Creator and Visual Studio
2. Open `src/soccerBotSoftware.pro` in Qt Creator
3. Build and run the project

## Generated Files

- **Run-DriverStation.ps1** - PowerShell launcher with advanced features
- **Run-DriverStation.bat** - Batch file launcher (simpler)
- **INSTALLATION.md** - Complete installation instructions
- **TROUBLESHOOTING.md** - Common issues and solutions

## Prerequisites

These scripts require:
- Qt Framework (5.12+) with qmake in PATH
- Visual Studio C++ build tools
- Windows 10/11

## Environment Notes

The current environment shows:
- Qt 5.15.2 is available via Anaconda distribution
- MSVC compiler (cl.exe) needs to be set up via:
  - Visual Studio Developer Command Prompt, or
  - Running vcvarsall.bat

## Automation Features

### PowerShell Script Features:
- Automatic Qt detection
- Project building (debug/release modes)
- Executable location scanning
- Error handling and reporting
- Build verification

### Batch File Features:
- Simple execution environment
- Automatic building if needed
- Basic error checking

## Usage Examples

### Build and Run (Release)
```powershell
.\Run-DriverStation.ps1 -Release
```

### Build Only (Debug)
```powershell
.\Run-DriverStation.ps1 -Build -Debug
```

### Run Existing Build
```cmd
Run-DriverStation.bat
```

## Project Structure

```
src/
├── soccerBotSoftware.pro     # Qt project file
├── main.cpp                  # Application entry point
├── mainwindow.cpp/h          # Main UI logic
├── mainwindow.ui             # UI design file
├── joystickhandler.cpp/h     # Joystick input handling
├── client.cpp/h              # Network client
├── host.cpp/h                # Network host
├── gamedata.cpp/h            # Game state data
├── robotinfo.cpp/h           # Robot information
└── udpsend.cpp/h             # UDP communication

build/                        # Build output directory
├── debug/                    # Debug build files
└── release/                  # Release build files
```

## Network Configuration

The DriverStation communicates with robots via UDP. Default configuration:
- **通信端口**: Check source code for port numbers
- **协议**: UDP broadcast/unicast
- **网络**: Same subnet as robots

## Development Notes

### Qt Version Compatibility
- Tested with Qt 5.15.2
- Should work with Qt 5.12+ (adjust as needed)
- Ensure QT += widgets and QT += network in .pro file

### Build System
- Uses qmake + nmake (MSVC)
- Can be configured for other build systems if needed
- CMake support could be added for cross-platform builds

### Dependencies
- **Qt Core/Gui/Widgets/Network**: UI framework
- **XInput**: Windows joystick support (linked in .pro)

## Security Notes

These scripts:
- Only access project directory and standard build tools
- Do not modify system files outside project
- Run with current user permissions
- Can be safely used in automated deployment workflows
