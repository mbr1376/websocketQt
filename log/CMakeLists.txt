
find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(log STATIC
    logger.cpp
    logger.h
)
target_link_libraries(log PUBLIC Qt6::Core)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
