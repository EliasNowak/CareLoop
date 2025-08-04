# CareLoop Health Monitor - Production Setup

## Overview

CareLoop is a Zephyr RTOS-based health monitoring device that provides:

- **Fall Detection** using MPU6050 accelerometer/gyroscope
- **Heart Rate Monitoring** using MAX30102 sensor 
- **BLE Connectivity** with GATT services
- **Battery Monitoring** 
- **LED Status Indicators**
- **Persistent Configuration Storage**

## Production Build

### Prerequisites

1. **Zephyr SDK** (v0.16.0 or later)
2. **West Tool** 
3. **CMake** (3.20.0 or later)
4. **Python 3.8+**
5. **Git**

### Environment Setup

```bash
# Set Zephyr environment
export ZEPHYR_BASE=/path/to/zephyr
source $ZEPHYR_BASE/zephyr-env.sh

# Initialize workspace (if needed)
west init -m https://github.com/zephyrproject-rtos/zephyr
west update
```

### Build Commands

#### Quick Production Build
```bash
./build_production.sh
```

#### Custom Board Build
```bash
./build_production.sh --board nrf52dk_nrf52832
```

#### Development Build
```bash
./build_production.sh --build-type development
```

#### Clean Build with Flash
```bash
./build_production.sh --clean --flash
```

### Manual Build (Alternative)

```bash
# Production build
west build -b nrf52840dk_nrf52840 --pristine \
  -DCONF_FILE=prj_production.conf \
  -DDTC_OVERLAY_FILE=boards/careloop_production.overlay

# Flash firmware
west flash
```

## Supported Hardware

### Primary Target
- **nRF52840 DK** (nrf52840dk_nrf52840)

### Alternative Targets
- **nRF52 DK** (nrf52dk_nrf52832)
- **nRF5340 DK** (nrf5340dk_nrf5340_cpuapp)

### Required Sensors
- **MAX30102** - Heart rate/SpO2 sensor (I2C: 0x57)
- **MPU6050** - Accelerometer/Gyroscope (I2C: 0x68)

### Pin Configuration
```
I2C0:
- SDA: P0.26
- SCL: P0.27

LEDs:
- Red LED:   P0.13 (Status/Alert)
- Green LED: P0.14 (Connected)
- Blue LED:  P0.15 (Advertising)

Button:
- User Button: P0.11 (Pull-up, Active-low)

ADC:
- Battery Monitor: Channel 0 (VDD)
```

## Project Structure

```
CareLoop/
├── src/
│   ├── main.c                    # Main application
│   ├── business/                 # Business logic
│   │   ├── fall_logic.c/h       # Fall detection algorithm
│   │   └── state_manager.c/h    # Device state management
│   ├── communication/           # BLE communication layer
│   │   ├── ble/                 # BLE management
│   │   ├── gatt/                # GATT services
│   │   └── include/             # Communication headers
│   ├── hal/                     # Hardware abstraction
│   │   ├── led.c/h              # LED control
│   │   ├── max30102.c/h         # Heart rate sensor
│   │   └── mpu6050.c/h          # Motion sensor
│   └── information/             # System information
│       ├── battery.c/h          # Battery monitoring
│       ├── config.c/h           # Configuration management
│       └── firmware_info.c/h    # Firmware information
├── boards/
│   └── careloop_production.overlay  # Production device tree
├── prj.conf                     # Development configuration
├── prj_production.conf          # Production configuration
├── build_production.sh          # Build script
└── CMakeLists.txt              # Build configuration
```

## BLE Services

### Device Information Service (0x180A)
- Manufacturer Name
- Model Number
- Firmware Revision
- Hardware Revision

### Heart Rate Service (0x180D)
- Heart Rate Measurement
- Body Sensor Location
- Heart Rate Control Point

### Custom Fall Detection Service
- Fall Alert Characteristic
- Fall Configuration
- Fall History

### Custom System Service  
- Battery Level
- Device Status
- Configuration Management

## Configuration Management

Settings are stored in NVS (Non-Volatile Storage) and include:

- Device name
- Fall detection threshold  
- Heart rate alert thresholds
- Power management mode

### Configuration API
```c
// Get/Set device name
const char *config_get_device_name(void);
int config_set_device_name(const char *name);

// Get/Set fall threshold
float config_get_fall_threshold(void);
int config_set_fall_threshold(float threshold);

// Save configuration
int config_save(void);
```

## Power Management

The device supports multiple power modes:

1. **Normal Mode** - Full functionality
2. **Eco Mode** - Reduced sensor sampling
3. **Performance Mode** - Enhanced responsiveness

Battery monitoring provides:
- Voltage measurement
- Percentage calculation  
- Low battery alerts
- Critical battery shutdown

## Fall Detection Algorithm

The fall detection uses:
- 3-axis accelerometer data
- Configurable G-force threshold
- Debouncing to prevent false positives
- Historical data analysis

### Algorithm Features
- Real-time magnitude calculation
- Circular buffer for sample history
- Configurable sensitivity
- False positive filtering

## Security Features

- **BLE Bonding** - Secure device pairing
- **Pin Code Protection** - Fixed or dynamic PIN
- **Data Encryption** - All BLE communication encrypted
- **Secure Boot** - MCUboot integration (production)

## Firmware Updates

Production builds include MCUboot for secure firmware updates:

- **Dual bank** - Safe fallback on failed updates
- **Signature verification** - Cryptographically signed images  
- **Version management** - Automatic rollback protection

## Testing

### Unit Tests
```bash
# Run unit tests (when implemented)
west build -t test
```

### Hardware Testing
1. Verify sensor communication
2. Test BLE connectivity
3. Validate fall detection
4. Check battery monitoring
5. Test configuration persistence

## Production Deployment

### Quality Assurance
1. Flash production firmware
2. Verify all sensors respond
3. Test BLE pairing and services
4. Validate fall detection sensitivity
5. Check battery monitoring accuracy
6. Verify configuration persistence

### Manufacturing Test
```bash
# Flash test firmware
west flash --hex-file artifacts/CareLoop_test_firmware.hex

# Run manufacturing tests
python scripts/manufacturing_test.py
```

## Troubleshooting

### Common Issues

**Build Errors:**
- Ensure Zephyr SDK is properly installed
- Check ZEPHYR_BASE environment variable
- Verify all dependencies are installed

**Flash Errors:**
- Check USB connection to dev board
- Ensure correct board target specified
- Try erasing flash: `west flash --erase`

**BLE Connection Issues:**
- Clear bonded devices
- Check device name in configuration
- Verify advertising is active

**Sensor Issues:**
- Check I2C connections
- Verify sensor addresses
- Review device tree configuration

### Debug Commands
```bash
# Enable debug logging
west build -DCONF_FILE=prj.conf

# Monitor RTT logs
JLinkRTTViewer

# GDB debugging
west debug
```

## Support

For technical support and issues:
- Check the troubleshooting section
- Review Zephyr documentation
- File issues on project repository

## License

Copyright (c) 2024 Nordic Semiconductor ASA
SPDX-License-Identifier: Apache-2.0
