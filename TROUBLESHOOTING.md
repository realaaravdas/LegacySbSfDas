# DriverStation Troubleshooting Guide

This guide covers common issues and solutions for the SoccerBot Software DriverStation application.

## Installation Issues

### "qmake is not recognized" Error

**Problem:** The build system cannot find qmake.

**Solutions:**
1. **Add Qt to PATH:**
   - Ensure Qt bin directory is in your system PATH
   - Typical path: `C:\Qt\5.15.2\msvc2019_64\bin`
   - Restart command prompt/PowerShell after updating PATH

2. **Use full path to qmake:**
   - Find qmake.exe in your Qt installation
   - Use absolute path in build commands

3. **Reinstall Qt with PATH option:**
   - Re-run Qt installer
   - Ensure "Add to PATH" option is selected

### "nmake is not recognized" Error

**Problem:** Visual Studio build tools are not properly configured.

**Solutions:**
1. **Install Visual Studio with C++ tools:**
   - Open Visual Studio Installer
   - Modify installation → add "Desktop development with C++"

2. **Use Developer Command Prompt:**
   - Start → "Developer Command Prompt for VS 2019/2022"
   - Run build commands from this prompt

3. **Add MSVC to PATH manually:**
   - Find vcvarsall.bat in your VS installation
   - Run: `call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64`

### Build Errors

#### "Cannot open include file: 'QtWidgets'"
**Problem:** Qt headers not found.

**Solutions:**
1. Verify Qt installation includes Qt modules
2. Check Qt Creator kit configuration
3. Rebuild project with correct kit selection

#### "undefined reference to vtable" errors
**Problem:** MOC file generation issues.

**Solutions:**
1. Clean build directory and rebuild:
   ```cmd
   rmdir /s build
   mkdir build
   cd build
   qmake ../src/soccerBotSoftware.pro
   nmake clean
   nmake
   ```

2. Check Q_OBJECT macro usage in header files
3. Ensure all header files are included in .pro file

## Runtime Issues

### Application Won't Launch

**Problem:** Double-clicking the executable does nothing.

**Causes & Solutions:**

1. **Missing Qt DLLs:**
   - Copy Qt5Core.dll, Qt5Gui.dll, Qt5Widgets.dll, Qt5Network.dll to executable directory
   - Use windeployqt tool: `windeployqt.exe soccerBotSoftware.exe`

2. **Missing Visual C++ Runtime:**
   - Install Microsoft Visual C++ Redistributable
   - Download from Microsoft's website

3. **Permission Issues:**
   - Run as Administrator
   - Check Windows Defender blocking

### "DLL not found" Errors

**Common Missing DLLs:**
```
- Qt5Core.dll
- Qt5Gui.dll  
- Qt5Widgets.dll
- Qt5Network.dll
- qwindows.dll (platform plugin)
- Xinput9_1_0.dll (joystick support)
```

**Solutions:**
1. **Copy from Qt installation:**
   ```cmd
   copy "C:\Qt\5.15.2\msvc2019_64\bin\Qt5*.dll" "build\release\"
   copy "C:\Qt\5.15.2\msvc2019_64\plugins\platforms\qwindows.dll" "build\release\platforms\"
   ```

2. **Use windeployqt:**
   ```cmd
   windeployqt.exe --release soccerBotSoftware.exe
   ```

3. **Add Qt to system PATH**

### GUI Issues

#### Black Window or No UI Elements
**Problem:** Application window appears but is black/blank.

**Solutions:**
1. Check graphics drivers
2. Run with OpenGL software rendering:
   ```cmd
   set QT_OPENGL=software
   soccerBotSoftware.exe
   ```

#### Window Won't Focus or Respond
**Problem:** GUI is unresponsive or can't be interacted with.

**Solutions:**
1. Check if application is frozen in Task Manager
2. Restart the application
3. Check for thread blocking issues in the code
4. Verify network connectivity if it's waiting for robot connection

## Network Communication Issues

### Cannot Connect to Robot

**Problem:** DriverStation cannot establish connection with robots.

**Troubleshooting Steps:**

1. **Check Network Configuration:**
   - Verify computer and robot are on same network
   - Ping robot IP address
   - Check firewall settings

2. **Verify Robot Status:**
   - Ensure robot is powered on
   - Check robot's network interface
   - Verify robot server software is running

3. **Check Port Availability:**
   - Default communication port may be blocked
   - Temporarily disable Windows Firewall
   - Add firewall exceptions for the application

4. **Test with Network Tools:**
   - Use `netstat -an | findstr :8080` (replace 8080 with actual port)
   - Use telnet or netcat to test connectivity

### Intermittent Connection Issues

**Problem:** Connection drops frequently or is unstable.

**Solutions:**
1. **Network Quality:**
   - Check WiFi signal strength
   - Try wired Ethernet connection
   - Check for network congestion

2. **Application Settings:**
   - Increase timeout values in application settings
   - Check UDP packet handling code
   - Verify network buffer sizes

## Joystick Input Issues

### Joystick Not Detected

**Problem:** Controller is connected but not recognized.

**Solutions:**
1. **Verify Joystick Connection:**
   - Check Windows Device Manager
   - Ensure controllers appear under "Human Interface Devices"

2. **Check XInput Support:**
   - Ensure -lXinput is linked (in .pro file)
   - Test with DirectX diagnostic tool (dxdiag)

3. **Permissions:**
   - Run as Administrator
   - Check Windows Game Bar interference

### Joystick Input Lag

**Problem:** Delayed response from joystick input.

**Solutions:**
1. Check USB connection quality
2. Try different USB port
3. Update joystick drivers
4. Check Windows power management settings

## Performance Issues

### High CPU Usage

**Problem:** Application uses excessive CPU resources.

**Causes & Solutions:**
1. **Inefficient Network Loops:**
   - Check for busy-wait loops in UDP handling
   - Implement proper sleep/delay mechanisms

2. **Qt Event Loop Issues:**
   - Check for blocking operations in main thread
   - Move heavy processing to worker threads

3. **GDI Leaks:**
   - Check for UI element resource leaks
   - Monitor handle count in Task Manager

### Memory Leaks

**Problem:** Memory usage continuously increases.

**Solutions:**
1. Use Qt's memory debugging tools
2. Check for circular references in QObject hierarchies
3. Verify proper cleanup in destructors

## Debugging Tools

### Application Logs

Enable debugging output:
1. Run from command line with verbose flags
2. Check Qt's qDebug() output
3. Monitor Windows Event Viewer for application crashes

### Network Debugging

Test network connectivity:
```cmd
# Test UDP connectivity
python server_test.py

# Monitor network traffic
netstat -an -p UDP
```

### Qt Creator Debugger

1. Open project in Qt Creator
2. Set breakpoints in problematic areas
3. Use the Application Output panel for runtime messages
4. Profile performance with Qt's performance tools

## Common Error Messages

### "This application failed to start because it could not find or load the Qt platform plugin"
**Solution:** Copy platform plugins from Qt installation
```cmd
xcopy "C:\Qt\5.15.2\msvc2019_64\plugins" "build\release\plugins\" /E /I
```

### "The procedure entry point could not be located in the dynamic link library"
**Solution:** Version mismatch between Qt DLLs and executable - rebuild with consistent Qt version.

### "Access violation reading location 0x00000000"
**Solution:** Null pointer dereference - use debugger to identify the problematic code section.

## Getting Help

If you continue to experience issues:

1. **Collect Diagnostic Information:**
   - Error messages (exact text)
   - System specifications
   - Step-by-step reproduction steps

2. **Update All Components:**
   - Latest graphics drivers
   - Latest Windows updates
   - Latest Qt version

3. **Community Resources:**
   - Qt Forum: https://forum.qt.io/
   - Stack Overflow with [qt] tag
   - GitHub Issues (if available)

4. **Team Support:**
   - Contact Saint Francis Robotics Team 2367 members
   - Check team documentation and shared resources
