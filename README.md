# WebSocket Qt Client & Server

This project provides a simple WebSocket client and server implementation using Qt6. The client is a Qt Widgets GUI application, and the server is a Qt-based WebSocket server that can broadcast real-time data and respond to client requests.

## Project Structure

## Features

- **Client**
  - Connect/disconnect to WebSocket server
  - Send requests and receive responses
  - Display real-time data from server

- **Server**
  - Accepts multiple WebSocket clients
  - Broadcasts real-time data periodically
  - Handles function calls from clients and sends results

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