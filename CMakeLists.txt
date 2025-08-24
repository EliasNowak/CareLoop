cmake_minimum_required(VERSION 3.20.0)

# Set the DTS root to include our custom bindings
set(DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(CareLoop)

# Main application sources
target_sources(app PRIVATE 
    src/main.c
)

if(CONFIG_MAX30102)
    target_sources(app PRIVATE src/drivers/sensor/max30102/max30102.c)
    target_include_directories(app PRIVATE src/drivers/sensor/max30102)
endif()
