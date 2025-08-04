# CareLoop Health Monitor - Nordic Connect SDK Build Instructions

## Overview

This project is now configured for **Nordic Connect SDK (NCS)** environment.

## Environment Setup

### 1. Set NCS Environment Variables

```bash
# Set your NCS installation path
export NCS_BASE=/home/elias/ncs/v3.0.2

# Add to ~/.bashrc for permanent setup
echo 'export NCS_BASE=/home/elias/ncs/v3.0.2' >> ~/.bashrc
```

### 2. Initialize NCS Environment

```bash
# Navigate to NCS installation
cd $NCS_BASE

# Activate the environment (if using venv)
source toolchains/*/activate.sh

# Or source the ncs environment
source zephyr/zephyr-env.sh
```

## Building the Project

### Quick Build Commands

```bash
# Development build
west build -b nrf52840dk/nrf52840

# Production build
west build -b nrf52840dk/nrf52840 -DCONF_FILE=prj_production.conf

# Clean build
west build -b nrf52840dk/nrf52840 --pristine

# Build and flash
west build -b nrf52840dk/nrf52840 && west flash
```

### Using the Build Script

```bash
# Make script executable
chmod +x build_production.sh

# Build for production
./build_production.sh

# Build for development
./build_production.sh --build-type development

# Clean build and flash
./build_production.sh --clean --flash
```

## Key Changes for NCS Compatibility

### Configuration Changes Made:

1. **Removed `CONFIG_BT_CONN=y`** - This is automatically enabled in NCS
2. **Updated board target** - Changed from `nrf52840dk_nrf52840` to `nrf52840dk/nrf52840`
3. **Simplified device tree** - Removed NCS-conflicting pinctrl configurations
4. **Updated build script** - Now uses `NCS_BASE` instead of `ZEPHYR_BASE`

### Files Updated:
- `prj.conf` - Main configuration file
- `prj_production.conf` - Production configuration
- `boards/careloop_production.overlay` - Device tree overlay
- `build_production.sh` - Build script
- `README_NCS.md` - This file

## Troubleshooting

### Common NCS Build Issues:

1. **"BT_CONN is assigned but not user-configurable"**
   - ✅ **Fixed** - Removed explicit `CONFIG_BT_CONN=y`

2. **Board target errors**
   - ✅ **Fixed** - Updated to use new board format `nrf52840dk/nrf52840`

3. **Environment not found**
   - Check that `NCS_BASE` is set correctly
   - Ensure you're in the NCS environment

### Environment Check:
```bash
# Verify NCS environment
echo $NCS_BASE
west --version
which west

# Should show something like:
# /home/elias/ncs/v3.0.2
# West version: v1.4.0
# /home/elias/ncs/toolchains/.../bin/west
```

## Development Workflow

### 1. Setup (One Time)
```bash
export NCS_BASE=/home/elias/ncs/v3.0.2
cd $NCS_BASE
source zephyr/zephyr-env.sh
```

### 2. Build and Test
```bash
cd /home/elias/repos/CareLoop
west build -b nrf52840dk/nrf52840 --pristine
west flash
```

### 3. Monitor Output
```bash
# Using west
west espressif monitor

# Or using screen (adjust device as needed)
screen /dev/ttyACM0 115200

# Or using minicom
minicom -D /dev/ttyACM0 -b 115200
```

## Hardware Requirements

- **nRF52840 DK** development board
- **MAX30102** heart rate sensor (I2C address: 0x57)
- **MPU6050** accelerometer/gyroscope (I2C address: 0x68)
- USB cable for programming and power

## Success Indicators

When the build completes successfully, you should see:
```
[100%] Built target zephyr
-- west build: build completed successfully
```

The firmware will:
1. Initialize all sensors
2. Start BLE advertising
3. Begin monitoring for heart rate and fall detection
4. Show status via LEDs

## Next Steps

Once building successfully:
1. Flash the firmware to your nRF52840 DK
2. Connect sensors via I2C
3. Use nRF Connect app to connect and test BLE services
4. Monitor logs for sensor data and fall detection events
