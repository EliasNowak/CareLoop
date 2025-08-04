cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(CareLoop)

# Include directories
target_include_directories(app PRIVATE
    src/communication/include
    src
)

# Main application
target_sources(app PRIVATE 
    src/main.c
)

# Business logic
target_sources(app PRIVATE
    src/business/fall_logic.c
    src/business/state_manager.c
)

# Communication layer
target_sources(app PRIVATE
    src/communication/ble/ble_manager.c
    src/communication/ble/ble_advertising.c
    src/communication/ble/ble_connection.c
    src/communication/ble/ble_security.c
    src/communication/gatt/gatt_server.c
    src/communication/gatt/services/fall_service.c
    src/communication/gatt/services/system_service.c
    src/communication/gatt/services/vitals_service.c
)

# Hardware Abstraction Layer
target_sources(app PRIVATE
    src/hal/led.c
    src/hal/max30102.c
    src/hal/mpu6050.c
)

# Information/Configuration
target_sources(app PRIVATE
    src/information/battery.c
    src/information/firmware_info.c
)
