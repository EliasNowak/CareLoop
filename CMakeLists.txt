cmake_minimum_required(VERSION 3.20.0)

# Set the DTS root to include our custom bindings
set(DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(CareLoop)

# Main application and driver sources
target_sources(app PRIVATE 
    src/main.c
    src/drivers/sensor/max30102/max30102.c
)

# Include our custom drivers
target_include_directories(app PRIVATE src/drivers/sensor/max30102)
