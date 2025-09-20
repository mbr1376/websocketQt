cmake_minimum_required(VERSION 3.5)

project(WebSocketClient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6WebSockets REQUIRED)

add_executable(WebSocketClientSample
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  websocketclient.cpp
  websocketclient.h
  decrypthelper.h decrypthelper.cpp
)

target_link_libraries(WebSocketClientSample PRIVATE Qt6::Widgets Qt6::WebSockets)
target_link_libraries(WebSocketClientSample PRIVATE encrypt)
