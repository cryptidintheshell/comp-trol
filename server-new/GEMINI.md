# Rat-Server

A Remote Administration Tool (RAT) server built with C++ and wxWidgets. This application provides a graphical interface to manage multiple remote client connections, send system commands, and transfer files.

## Project Overview

The `rat-server` acts as a central controller that listens for incoming TCP connections from client applications. Once a client connects, it is displayed in a management grid where the operator can select it and perform various administrative tasks.

### Key Features
- **Multi-Client Support**: Handles multiple simultaneous client connections using threads.
- **System Commands**: Send predefined triggers for Shutdown, Restart, Lock, and Closing programs.
- **File Transfer**: Send files from the server to connected clients.
- **Real-time Logging**: Status updates and connection events are logged in the UI and terminal.
- **Heartbeat System**: Basic "ping-pong" mechanism to monitor connection health.

## Tech Stack
- **Language**: C++
- **UI Framework**: wxWidgets
- **Networking**: BSD Sockets (TCP)
- **Build System**: GNU Make

## Directory Structure
- `src/`: Source code files.
  - `app.cpp`: Application entry point and initialization.
  - `gui.cpp`: Main window definition. Includes other `.cpp` files for modularity.
  - `socket.cpp`: Socket initialization and connection handling.
  - `buttons.cpp`: Event handlers for UI buttons and command logic.
  - `gui-functions.cpp`: UI component setup and layout.
- `headers/`: Header files.
  - `app.h`: `App` class declaration.
  - `window.h`: `Window` class and `ClientComputer` struct declarations.
- `build/`: Destination for compiled object files.
- `Makefile`: Build instructions.

## Building and Running

### Prerequisites
- `g++` compiler
- `wxWidgets` library (and `wx-config` utility)

### Compilation
To build the project, run:
```bash
make
```

### Execution
After building, run the generated binary:
```bash
./rat-server
```

### Cleanup
To remove compiled object files:
```bash
make clean
```

## Architectural Notes & Conventions

### Modularity via Inclusion
This project uses a specific pattern where implementation files (`socket.cpp`, `buttons.cpp`, `gui-functions.cpp`) are directly included in `src/gui.cpp` via `#include`. This results in a single compilation unit for the `Window` class implementation.

### Naming Conventions
- **UI Components**: 
  - `btn*`: Buttons (e.g., `btnStart`)
  - `pnl*`: Panels (e.g., `pnlMain`)
  - `szr*`: Sizers (e.g., `szrMain`)
  - `grd*`: Grids (e.g., `grdClients`)
  - `txtctrl*`: Text Controls (e.g., `txtctrlUpdateField`)
- **Member Variables**: snake_case (e.g., `client_sockets`).
- **Methods**: PascalCase (e.g., `HandleIncomingConnection`).

### Network Protocol
- **Port**: 5953 (Default)
- **Command Codes**:
  - `229892`: Shutdown
  - `893234`: Restart
  - `837453`: Lock
  - `764853`: Close Programs
  - `993123`: Initiate File Transfer
- **Keep-Alive**: "ping" / "pong" exchange.

## Workflow
1. **Launch**: Start the application.
2. **Initialize**: Click the "Start" button to begin listening for connections on port 5953.
3. **Connect**: Clients connect and provide a 3-character ID.
4. **Manage**: Select a client from the grid.
5. **Execute**: Use the command buttons or "Send file" to interact with the selected client.
