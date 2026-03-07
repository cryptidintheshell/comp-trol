# CompMan: A Remote Computer Management Tool

A multi-component remote administration and management system designed for host-client computer control. This project provides a desktop server application, a Windows-based client, and an alternative control interface through a NodeMCU (ESP8266).

## Components

### 1. Server Application (`server-new/`)
The main control hub for managing remote clients.
- **Technology:** C++, wxWidgets (GUI), POSIX Sockets.
- **Functionality:**
  - Multi-threaded handling of incoming client connections.
  - Graphical interface to monitor connected clients (ID and IP).
  - Control panel for sending remote commands to specific clients.
  - File transfer capabilities (in development).
- **Default Port:** 5953

### 2. Client Application (`client-new/`)
A background service designed to run on Windows target machines.
- **Technology:** C++, Win32 API, Winsock2.
- **Functionality:**
  - Background execution with console hiding.
  - Automatic persistence by copying itself to the Windows Startup folder.
  - Non-blocking socket communication.
  - Execution of system-level commands received from the server.
- **Default Port:** 8080 (Targeting Server/NodeMCU IP)

### 3. NodeMCU Controller (`nodemcu-socket/`)
An alternative hardware-based control interface.
- **Technology:** Arduino (C++), ESP8266WiFi, ESPAsyncWebServer.
- **Functionality:**
  - Acts as a bridge or standalone server for clients.
  - Provides a web-based dashboard (Port 80) for remote management via mobile or browser.
  - Supports quick actions like remote shutdown and restart.

## Features

- **Client Persistence:** The client automatically installs itself to the Windows Startup directory upon execution.
- **Stealth Mode:** The client can run hidden from the user's desktop.
- **Real-time Monitoring:** The server tracks connection status and provides feedback on command execution.
- **System Control:** Remote execution of shutdown, restart, and workstation locking.
- **Cross-Platform Control:** Manage Windows clients from a Linux-based server or any device with a web browser (via NodeMCU).

## Remote Command Codes

**NOTE:** Connection & communication security will be implemented after the project's main functions are added
The system uses specific numeric codes for communication between the server and clients.

- **229892:** Shutdown system
- **893234:** Restart system
- **126773 / 837453:** Lock workstation
- **764853:** Close programs (Server-side)
- **993123:** File transfer initialization

## Building the Project

### Prerequisites
- **Server:** Linux environment with `g++` and `wxWidgets` (development headers).
- **Client:** `mingw-w64` cross-compiler for building Windows executables on Linux.
- **NodeMCU:** Arduino IDE or PlatformIO with ESP8266 board support.

### Server Compilation
```bash
cd server-new
make
```

### Client Compilation
```bash
cd client-new
make
```

### NodeMCU Setup
1. Open `nodemcu-socket/nodemcu-socket.ino` in the Arduino IDE.
2. Update the `ssid` and `password` variables with your network credentials.
3. Flash the code to your ESP8266 module.

## Notes
- Ensure that firewall settings allow communication on ports 5953 and 8080.
- The client application must be configured with the correct Server/NodeMCU IP address before compilation.
