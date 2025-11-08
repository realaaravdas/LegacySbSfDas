# DriverStation Launcher for SoccerBot Software
# This script launches the DriverStation application with proper error handling

param(
    [switch]$Build, 
    [switch]$Release,
    [switch]$Debug
)

$ErrorActionPreference = "Stop"

# Configuration
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$SourceDir = Join-Path $ProjectRoot "src"
$BuildDir = Join-Path $ProjectRoot "build"
$ExecutableName = "soccerBotSoftware.exe"

Write-Host "SoccerBot DriverStation Launcher" -ForegroundColor Green
Write-Host "Project Root: $ProjectRoot" -ForegroundColor Cyan

# Function to check if Qt is installed
function Test-QtInstallation {
    try {
        $qmake = & qmake -v 2>&1
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Qt found:" -ForegroundColor Green
            Write-Host $qmake[0] -ForegroundColor Gray
            return $true
        }
    }
    catch {
        # Continue to next check
    }
    
    try {
        $qmakePath = Get-Command "qmake.exe" -ErrorAction SilentlyContinue
        if ($qmakePath) {
            Write-Host "Qt found at: $($qmakePath.Source)" -ForegroundColor Green
            return $true
        }
    }
    catch {
        # Continue to next check
    }
    
    return $false
}

# Function to build the project
function Build-Project {
    param($BuildType = "debug")
    
    Write-Host "Building project in $BuildType mode..." -ForegroundColor Yellow
    
    # Create build directory
    if (-not (Test-Path $BuildDir)) {
        New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null
    }
    
    Set-Location $BuildDir
    
    try {
        # Configure project
        if ($BuildType -eq "release") {
            & qmake "../src/soccerBotSoftware.pro" CONFIG+=release
        }
        else {
            & qmake "../src/soccerBotSoftware.pro" CONFIG+=debug
        }
        
        if ($LASTEXITCODE -ne 0) {
            throw "qmake configuration failed"
        }
        
        # Build project
        & nmake
        if ($LASTEXITCODE -ne 0) {
            throw "Build failed with nmake"
        }
        
        Write-Host "Build completed successfully!" -ForegroundColor Green
        return $true
    }
    catch {
        Write-Host "Build failed: $_" -ForegroundColor Red
        return $false
    }
    finally {
        Set-Location $ProjectRoot
    }
}

# Function to find executable
function Find-Executable {
    # Check common build locations
    $locations = @(
        (Join-Path $BuildDir "release" $ExecutableName),
        (Join-Path $BuildDir "debug" $ExecutableName),
        (Join-Path $BuildDir $ExecutableName),
        (Join-Path $SourceDir "release" $ExecutableName),
        (Join-Path $SourceDir "debug" $ExecutableName),
        (Join-Path $SourceDir $ExecutableName)
    )
    
    foreach ($location in $locations) {
        if (Test-Path $location) {
            return $location
        }
    }
    
    return $null
}

# Main execution
try {
    # Check Qt installation
    if (-not (Test-QtInstallation)) {
        Write-Host "Error: Qt is not installed or not in PATH" -ForegroundColor Red
        Write-Host "Please install Qt with Qt Creator and ensure qmake is in your PATH" -ForegroundColor Yellow
        Write-Host "Download from: https://www.qt.io/download" -ForegroundColor Cyan
        exit 1
    }
    
    # Build if requested
    if ($Build -or $Release -or $Debug) {
        $buildType = if ($Release) { "release" } else { "debug" }
        if (-not (Build-Project -BuildType $buildType)) {
            exit 1
        }
    }
    
    # Find executable
    $executablePath = Find-Executable
    if (-not $executablePath) {
        Write-Host "Executable not found. Attempting to build..." -ForegroundColor Yellow
        
        if (Build-Project -BuildType "debug") {
            $executablePath = Find-Executable
        }
        
        if (-not $executablePath) {
            Write-Host "Could not find or build the executable" -ForegroundColor Red
            Write-Host "Try running with -Build parameter to force a build" -ForegroundColor Yellow
            exit 1
        }
    }
    
    Write-Host "Launching DriverStation from: $executablePath" -ForegroundColor Green
    
    # Check for required Qt DLLs
    $qtCore = Get-Command "Qt5Core.dll" -ErrorAction SilentlyContinue
    if (-not $qtCore -and -not (Test-Path (Join-Path $BuildDir "Qt5Core.dll"))) {
        Write-Host "Warning: Qt DLLs may not be in PATH" -ForegroundColor Yellow
        Write-Host "If the application fails to start, ensure Qt binaries are in your PATH" -ForegroundColor Yellow
        Write-Host "or copy Qt DLLs to the same directory as the executable" -ForegroundColor Yellow
    }
    
    # Launch the application
    Start-Process -FilePath $executablePath -WorkingDirectory (Split-Path $executablePath)
    
    Write-Host "DriverStation launched successfully!" -ForegroundColor Green
}
catch {
    Write-Host "Error: $_" -ForegroundColor Red
    exit 1
}
