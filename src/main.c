/*
 * CareLoop - Minimal HAL Heart Rate Monitor
 * Copyright (c) 2024
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "hal_sensor.h"
#include "hal_max30102.h"

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#define MPU6050_NODE DT_NODELABEL(mpu6050)

void read_mpu6050(void) {
    const struct device *mpu = DEVICE_DT_GET(MPU6050_NODE);
    struct sensor_value accel[3], gyro[3];

    if (!device_is_ready(mpu)) {
        printk("MPU6050 not ready\n");
        return;
    }

    sensor_sample_fetch(mpu);
    sensor_channel_get(mpu, SENSOR_CHAN_ACCEL_XYZ, accel);
    sensor_channel_get(mpu, SENSOR_CHAN_GYRO_XYZ, gyro);

    printk("Accel: %d %d %d\n", accel[0].val1, accel[1].val1, accel[2].val1);
    printk("Gyro: %d %d %d\n", gyro[0].val1, gyro[1].val1, gyro[2].val1);
}

LOG_MODULE_REGISTER(careloop, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("CareLoop HAL Heart Rate Monitor");
    
    hal_error_t ret;
    hal_sensor_t *sensor;
    hal_sensor_reading_t reading;
    
    /* Initialize HAL system */
    ret = hal_max30102_init();
    if (ret != HAL_OK) {
        LOG_ERR("HAL init failed: %d", ret);
        return -1;
    }
    
    ret = hal_sensor_init_all();
    if (ret != HAL_OK) {
        LOG_ERR("Sensor init failed: %d", ret);
        return -1;
    }
    
    /* Get heart rate sensor */
    sensor = hal_sensor_get(HAL_SENSOR_TYPE_HEART_RATE);
    if (!sensor) {
        LOG_ERR("No heart rate sensor found");
        return -1;
    }
    
    LOG_INF("Heart rate sensor ready");
    
    /* Main reading loop */
    while (1) {
        ret = sensor->ops->read(&reading);
        if (ret == HAL_OK) {
            if (reading.quality >= HAL_QUALITY_FAIR) {
                LOG_INF("HR: %d (Q:%d%%)", reading.raw_value, reading.quality);
            }
        }

        /* Read and print MPU6050 sensor data */
        read_mpu6050();

        k_sleep(K_MSEC(200));
    }
    
    return 0;
}
