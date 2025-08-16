/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include "drivers/sensor/max30102/max30102.h"

LOG_MODULE_REGISTER(careloop, LOG_LEVEL_DBG);

#define MAX30102_NODE DT_NODELABEL(max30102)
#define I2C_NODE DT_NODELABEL(i2c0)

static void i2c_scan(const struct device *i2c_dev)
{
    uint8_t cnt = 0, first = 0x04, last = 0x77;

    LOG_INF("I2C Bus Scan:");
    LOG_INF("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
    
    for (uint8_t i = 0; i <= last; i += 16) {
        printk("%02x: ", i);
        for (uint8_t j = 0; j < 16; j++) {
            if (i + j < first || i + j > last) {
                printk("   ");
                continue;
            }

            struct i2c_msg msgs[1];
            uint8_t dst = 1;

            msgs[0].buf = &dst;
            msgs[0].len = 0U;
            msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
            
            int result = i2c_transfer(i2c_dev, &msgs[0], 1, i + j);
            if (result == 0) {
                printk("%02x ", i + j);
                ++cnt;
                if (i + j == 0x57) {
                    LOG_INF("*** Found MAX30102 at address 0x57! ***");
                }
            } else {
                printk("-- ");
                if (i + j == 0x57) {
                    LOG_WRN("No response from MAX30102 at 0x57 (error: %d)", result);
                }
            }
        }
        printk("\n");
    }
    
    LOG_INF("Found %d device(s) on I2C bus", cnt);
    
    if (cnt == 0) {
        LOG_ERR("No I2C devices found! Check connections:");
        LOG_ERR("- MAX30102 VCC → nRF52840 3.3V");
        LOG_ERR("- MAX30102 GND → nRF52840 GND");  
        LOG_ERR("- MAX30102 SDA → nRF52840 P0.26");
        LOG_ERR("- MAX30102 SCL → nRF52840 P0.27");
        LOG_ERR("- Ensure 3.3V power supply (NOT 5V)");
        LOG_ERR("- Check wire connections and continuity");
    }
}

int main(void)
{
    LOG_INF("=== CareLoop MAX30102 Integration Test ===");
    
    /* Check I2C bus first */
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
    
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }
    
    LOG_INF("I2C device ready");
    
    /* Scan I2C bus for devices */
    i2c_scan(i2c_dev);
    
    /* Get the MAX30102 device */
    const struct device *max30102_dev = DEVICE_DT_GET(MAX30102_NODE);
    
    if (!device_is_ready(max30102_dev)) {
        LOG_ERR("MAX30102 device not ready");
        LOG_INF("Check:");
        LOG_INF("1. Hardware connections (VCC, GND, SDA, SCL)");
        LOG_INF("2. I2C address 0x57 appears in scan above");
        LOG_INF("3. Sensor power supply (3.3V)");
        return -1;
    }
    
    LOG_INF("MAX30102 device ready! Starting sensor readings...");
    
    struct sensor_value red_val, ir_val;
    int ret;
    uint32_t sample_count = 0;
    
    while (1) {
        /* Trigger a sensor reading */
        ret = sensor_sample_fetch(max30102_dev);
        if (ret) {
            LOG_ERR("Failed to fetch sample (err %d)", ret);
            k_sleep(K_SECONDS(1));
            continue;
        }
        
        /* Get the red LED channel data */
        ret = sensor_channel_get(max30102_dev, SENSOR_CHAN_RED, &red_val);
        if (ret) {
            LOG_ERR("Failed to get red channel (err %d)", ret);
        } else {
            LOG_INF("Sample #%d - Red: %d, IR: TBD", sample_count, red_val.val1);
        }
        
        /* Get the IR LED channel data */
        ret = sensor_channel_get(max30102_dev, SENSOR_CHAN_IR, &ir_val);
        if (ret) {
            LOG_ERR("Failed to get IR channel (err %d)", ret);
        } else {
            LOG_INF("Sample #%d - Red: %d, IR: %d", sample_count, red_val.val1, ir_val.val1);
        }
        
        sample_count++;
        
        /* Sleep for 1 second */
        k_sleep(K_SECONDS(1));
    }
    
    return 0;
}
