
find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(log STATIC
    logger.cpp
    logger.h
)

if(ENABLE_DB_LOG)
    find_package(Qt6 REQUIRED COMPONENTS Sql)
    target_link_libraries(log PRIVATE Qt6::Sql)
endif()
target_link_libraries(log PUBLIC Qt6::Core)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(ENABLE_FILE_LOG)
    add_definitions(-DUSE_FILE_LOG)
    message(STATUS "Logging to File is ENABLED")

endif()

if(ENABLE_DB_LOG)
    add_definitions(-DUSE_DB_LOG)
    find_package(Qt6 REQUIRED COMPONENTS Sql)
    message(STATUS "Logging to  DB is ENABlED")


endif()
