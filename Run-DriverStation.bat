@echo off
REM DriverStation Launcher (Batch Version)
REM This batch file launches the SoccerBot Software DriverStation

setlocal enabledelayedexpansion

echo SoccerBot DriverStation Launcher
echo =================================

set PROJECT_ROOT=%~dp0
set SOURCE_DIR=%PROJECT_ROOT%src
set BUILD_DIR=%PROJECT_ROOT%build
set EXECUTABLE_NAME=soccerBotSoftware.exe

echo Project Root: %PROJECT_ROOT%

REM Check if Qt is available
where qmake >nul 2>&1
if errorlevel 1 (
    echo Error: Qt is not installed or not in PATH
    echo Please install Qt with Qt Creator and ensure qmake is in your PATH
    echo Download from: https://www.qt.io/download
    pause
    exit /b 1
)

echo Qt found successfully

REM Try to find existing executable
set EXECUTABLE_PATH=
if exist "%BUILD_DIR%\release\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%BUILD_DIR%\release\%EXECUTABLE_NAME%
) else if exist "%BUILD_DIR%\debug\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%BUILD_DIR%\debug\%EXECUTABLE_NAME%
) else if exist "%BUILD_DIR%\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%BUILD_DIR%\%EXECUTABLE_NAME%
) else if exist "%SOURCE_DIR%\release\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%SOURCE_DIR%\release\%EXECUTABLE_NAME%
) else if exist "%SOURCE_DIR%\debug\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%SOURCE_DIR%\debug\%EXECUTABLE_NAME%
) else (
    echo Executable not found. Attempting to build...
    goto :build_project
)

goto :run_application

:build_project
echo Creating build directory...
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cd /d "%BUILD_DIR%"

echo Configuring project...
qmake "%SOURCE_DIR%\soccerBotSoftware.pro"
if errorlevel 1 (
    echo qmake configuration failed
    pause
    exit /b 1
)

echo Building project...
nmake
if errorlevel 1 (
    echo Build failed with nmake
    cd /d "%PROJECT_ROOT%"
    pause
    exit /b 1
)

echo Build completed successfully!

REM Check for executable after build
if exist "release\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%BUILD_DIR%\release\%EXECUTABLE_NAME%
) else if exist "debug\%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%BUILD_DIR%\debug\%EXECUTABLE_NAME%
) else if exist "%EXECUTABLE_NAME%" (
    set EXECUTABLE_PATH=%BUILD_DIR%\%EXECUTABLE_NAME%
)

if "%EXECUTABLE_PATH%"=="" (
    echo Could not find executable after build
    cd /d "%PROJECT_ROOT%"
    pause
    exit /b 1
)

cd /d "%PROJECT_ROOT%"

:run_application
echo Launching DriverStation from: %EXECUTABLE_PATH%
echo.

REM Check for Qt DLLs warning
where Qt5Core.dll >nul 2>&1
if errorlevel 1 (
    echo Warning: Qt DLLs may not be in PATH
    echo If the application fails to start, ensure Qt binaries are in your PATH
    echo or copy Qt DLLs to the same directory as the executable
    echo.
)

REM Launch the application
start "" /D "%~dp0%EXECUTABLE_PATH%" "%EXECUTABLE_PATH%"

echo DriverStation launched successfully!
pause
