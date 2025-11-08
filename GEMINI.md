# Project Overview

This project contains the server software for a custom-built soccer robot control system. The software is designed to run on a Raspberry Pi, which is connected to an Arduino Yun. The system uses a custom UDP-based communication protocol to receive commands from a host computer and control the robot's motors via PWM signals.

The main components of the project are:

*   **`src/pi/server.py`**: The main server script that runs on the Raspberry Pi. It handles network communication, motor control, and threading.
*   **`src/pi/transform.py`**: A utility script that converts joystick input values into PWM signals for the motors.
*   **`src/installPiServer.sh`**: An installation script that sets up the server on a Raspberry Pi.
*   **`legacy/`**: A directory containing what appears to be an older, C++ based version of the control system.

# Building and Running

The project is designed to be run on a Raspberry Pi. The following steps are required to install and run the server:

1.  **Run the installation script:**

    ```bash
    curl -s https://raw.githubusercontent.com/Saint-Francis-Robotics-Team2367/yunServerSoftware/master/src/installPiServer.sh | bash
    ```

2.  **Reboot the Raspberry Pi.** The server will start automatically on boot.

The installation script also sets up a `config.ini` file, where the team name should be configured.

# Development Conventions

*   The project is written in Python 2.
*   The server uses a multi-threaded architecture to handle network communication and motor control concurrently.
*   The project uses the `Adafruit_PWM_Servo_Driver` library to control PWM outputs.
*   The project uses a custom UDP-based communication protocol.
*   The `legacy` directory contains C++ code that appears to be an older version of the project. It is not clear if this code is still in use.
