cmake_minimum_required(VERSION 3.5)

project(WebSocketClientQt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_subdirectory(websocketclient)
add_subdirectory(websocketServer)
add_subdirectory(encrypt)
