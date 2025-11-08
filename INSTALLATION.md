# DriverStation Installation Guide

## Overview
This guide covers the complete installation and setup process for the SoccerBot Software DriverStation, a C++ Qt-based application for controlling robots in the Saint Francis Robotics Team 2367.

## System Requirements

### Software Requirements
- **Windows 10 or 11** (64-bit recommended)
- **Microsoft Visual Studio** (2019 or later, with C++ tools)
- **Qt Framework** (Qt 5.12 or later)
- **Git** (for cloning/downloading the source code)

### Hardware Requirements
- Minimum 4GB RAM (8GB recommended)
- 2GB free disk space
- Network connection (for robot communication)

## Step-by-Step Installation

### 1. Install Microsoft Visual Studio

1. Download Visual Studio from [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/)
2. Run the installer and select **"Desktop development with C++"**
3. Ensure the following components are included:
   - MSVC C++ build tools
   - Windows SDK
   - CMake tools (optional but recommended)

### 2. Install Qt Framework with Qt Creator

1. Download Qt from [https://www.qt.io/download](https://www.qt.io/download)
2. Choose the **Open Source** version (free)
3. Select Qt 5.12.x or later (5.15.x recommended)
4. During installation, ensure these components are selected:
   - **Qt Creator** IDE
   - **Qt 5.12.x (or 5.15.x)** for your compiler (MSVC 2019/2022)
   - **Qt 5.12.x (or 5.15.x) Sources** (for debugging)
   - **Qt 5.12.x (or 5.15.x) Tools**

### 3. Configure Environment Variables

After installing Qt, you need to add it to your system PATH:

1. Open **System Properties** → **Advanced** → **Environment Variables**
2. Under **System variables**, find and edit the **Path** variable
3. Add the following paths (adjust version numbers as needed):
   ```
   C:\Qt\5.15.2\msvc2019_64\bin
   C:\Qt\Tools\QtCreator\bin
   ```
4. Click OK on all windows

### 4. Set Up Project Source Code

#### Option A: Clone from Git Repository
```cmd
git clone <repository-url> LegacySbSfDas
cd LegacySbSfDas
```

#### Option B: Download Source Files
1. Extract the source code to your desired location
2. Navigate to the project directory using Command Prompt or PowerShell

### 5. Build the Application

#### Using PowerShell (Recommended)
1. Open PowerShell as Administrator
2. Navigate to the project directory:
   ```powershell
   cd "C:\Users\r2d2d\Documents\Dev\Robotics\LegacySbSfDas"
   ```
3. Run the launcher with build option:
   ```powershell
   .\Run-DriverStation.ps1 -Build
   ```

#### Using Command Prompt
1. Open Command Prompt as Administrator
2. Navigate to the project directory:
   ```cmd
   cd C:\Users\r2d2d\Documents\Dev\Robotics\LegacySbSfDas
   ```
3. Run the batch launcher:
   ```cmd
   Run-DriverStation.bat
   ```

#### Building Manually (Alternative)
If the automated scripts don't work, build manually:
1. Open **Qt Creator**
2. File → Open File or Project
3. Select `src/soccerBotSoftware.pro`
4. Configure the project with your kit (MSVC 2019 64-bit)
5. Build → Build Project (Ctrl+B)

### 6. Run the Application

After successful build, you can run the DriverStation using either launcher:

#### PowerShell Method
```powershell
.\Run-DriverStation.ps1
```

#### Batch File Method
```cmd
Run-DriverStation.bat
```

#### Direct Executable
Navigate to the build directory and run `soccerBotSoftware.exe`

## Installation Verification

To verify your installation:
1. Run the launcher scripts - they should start the DriverStation GUI
2. The application should open without DLL missing errors
3. Check that all UI elements load correctly
4. Verify network connectivity tools are available

## Platform-Specific Notes

### Windows Defender Issues
If Windows Defender blocks the application:
1. Go to Windows Security → Virus & threat protection
2. Add an exception for the project directory
3. Add an exception for the built executable

### Antivirus Software
Some antivirus software may flag Qt applications. Add exceptions if needed.

### Permission Issues
Run PowerShell/Command Prompt as Administrator if you encounter permission errors.

## Required Qt DLLs

When deploying the application, ensure these Qt DLLs are available:
- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll
- Qt5Network.dll
- platform/qwindows.dll

These can be found in your Qt installation directory under:
```
C:\Qt\5.15.2\msvc2019_64\bin\
```

## Creating a Portable Distribution

To create a standalone executable package:
1. Build the application in Release mode
2. Copy all required Qt DLLs to the executable directory
3. Use `windeployqt.exe` tool:
   ```cmd
   windeployqt.exe soccerBotSoftware.exe
   ```
4. Test the portable version on a clean Windows system

## Next Steps

After successful installation:
- Read the **TROUBLESHOOTING.md** guide for common issues
- Configure your robot connection settings
- Test the joystick input system
- Verify network communication with robots
