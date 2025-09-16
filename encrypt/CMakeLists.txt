find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(encrypt STATIC
    qaesencryption.cpp
    qaesencryption.h
)
target_link_libraries(encrypt PUBLIC Qt6::Core)
target_include_directories(encrypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
