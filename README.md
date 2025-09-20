# WebSocket Qt Client & Server

This project provides a simple WebSocket client and server implementation using Qt6. The client is a Qt Widgets GUI application, and the server is a Qt-based WebSocket server that can broadcast real-time data and respond to client requests.

## Project Structure

```
websocketQt/
├── encrypt/           # AES encryption/decryption helper (Qt-AES)
│   ├── qaesencryption.h
│   ├── qaesencryption.cpp
│   ├── encrypthelper.h
│   ├── encrypthelper.cpp
│   ├── decrypthelper.h
│   └── decrypthelper.cpp
├── websocketClient/   # Qt GUI client
├── websocketServer/   # Qt WebSocket server
├── CMakeLists.txt
└── ...
```

## Features

- **Client**
  - Connect/disconnect to WebSocket server
  - Send requests and receive responses
  - Display real-time data from server
  - **Decrypts received data using AES**

- **Server**
  - Accepts multiple WebSocket clients
  - Broadcasts real-time data periodically
  - Handles function calls from clients and sends results
  - **Encrypts outgoing data using AES**

- **Encryption**
  - Uses [Qt-AES](https://github.com/bricke/Qt-AES) for AES-128/CBC encryption and decryption
  - Helper classes (`EncryptHelper`, `DecryptHelper`) for easy usage

## Build Instructions

### Prerequisites

- Qt 6.x (with Widgets and WebSockets modules)
- CMake 3.5 or higher
- C++ compiler (supporting C++11 or higher)

### Build Steps

1. Clone the repository:
    ```sh
    git clone <your-repo-url>
    cd websocketQt
    ```

2. Create a build directory and run CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    ```

3. Build the project:
    ```sh
    cmake --build .
    ```

## Running

### Start the Server

From the `build/websocketServer` directory (or wherever your build output is):

```sh
./websocketServer
```

### Encryption/Decryption Usage

- All sensitive data sent from the server is **encrypted** using `EncryptHelper` (AES-128/CBC, base64).
- The client **decrypts** incoming data using `DecryptHelper` with the same key and IV.
- To change the encryption key/IV, edit them in `encrypthelper.cpp` and `decrypthelper.cpp` (must match on both sides).

---

**For more details, see the `encrypt/` directory and usage examples in client/server source code.**