/*
 * CareLoop Hardware Abstraction Layer - Sensor Interface
 * Copyright (c) 2024
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H

#include "hal_common.h"

/**
 * @brief Sensor types supported by the HAL
 */
typedef enum {
    HAL_SENSOR_TYPE_HEART_RATE = 0,
    HAL_SENSOR_TYPE_SPO2,
    HAL_SENSOR_TYPE_GYRO,
    HAL_SENSOR_TYPE_ACCEL,
    HAL_SENSOR_TYPE_COUNT
} hal_sensor_type_t;

/**
 * @brief Sensor reading structure
 */
typedef struct {
    hal_timestamp_t timestamp;     /**< Timestamp when reading was taken */
    float value;                   /**< Sensor value (units depend on type) */
    uint32_t raw_value;           /**< Raw ADC value */
    /* Optional vector components for multi-axis sensors (set when relevant) */
    float x;
    float y;
    float z;
    hal_quality_t quality;        /**< Signal quality (0-100%) */
    hal_error_t error_code;       /**< Error code if reading failed */
} hal_sensor_reading_t;

/**
 * @brief Sensor configuration structure
 */
typedef struct {
    uint32_t sample_rate_hz;      /**< Sampling rate in Hz */
    uint8_t led_current;          /**< LED current (0-255) */
    uint16_t adc_range;           /**< ADC range setting */
    uint16_t pulse_width_us;      /**< Pulse width in microseconds */
    bool auto_calibrate;          /**< Enable auto-calibration */
} hal_sensor_config_t;

/**
 * @brief Sensor statistics structure
 */
typedef struct {
    uint32_t total_samples;       /**< Total samples taken */
    uint32_t valid_samples;       /**< Valid samples count */
    uint32_t error_count;         /**< Error count */
    hal_quality_t avg_quality;   /**< Average signal quality */
    hal_timestamp_t last_reading; /**< Timestamp of last reading */
} hal_sensor_stats_t;

/**
 * @brief Sensor operations structure (function pointers)
 */
typedef struct {
    /**
     * @brief Initialize the sensor
     * @return HAL_OK on success, negative error code on failure
     */
    hal_error_t (*init)(void);
    
    /**
     * @brief Read sensor data
     * @param reading Pointer to store the reading
     * @return HAL_OK on success, negative error code on failure
     */
    hal_error_t (*read)(hal_sensor_reading_t *reading);
    
    /**
     * @brief Configure the sensor
     * @param config Pointer to configuration structure
     * @return HAL_OK on success, negative error code on failure
     */
    hal_error_t (*configure)(const hal_sensor_config_t *config);
    
    /**
     * @brief Calibrate the sensor
     * @return HAL_OK on success, negative error code on failure
     */
    hal_error_t (*calibrate)(void);
    
    /**
     * @brief Get sensor status
     * @return Current device status
     */
    hal_device_status_t (*get_status)(void);
    
    /**
     * @brief Get sensor statistics
     * @param stats Pointer to store statistics
     * @return HAL_OK on success, negative error code on failure
     */
    hal_error_t (*get_stats)(hal_sensor_stats_t *stats);
    
    /**
     * @brief Reset sensor statistics
     * @return HAL_OK on success, negative error code on failure
     */
    hal_error_t (*reset_stats)(void);
} hal_sensor_ops_t;

/**
 * @brief Sensor instance structure
 */
typedef struct {
    hal_sensor_type_t type;       /**< Sensor type */
    const char *name;             /**< Sensor name string */
    const hal_sensor_ops_t *ops;  /**< Operations function pointers */
    void *priv_data;             /**< Private data pointer */
    bool initialized;            /**< Initialization status */
} hal_sensor_t;

/**
 * @brief Register a sensor with the HAL
 * @param sensor Pointer to sensor instance
 * @return HAL_OK on success, negative error code on failure
 */
hal_error_t hal_sensor_register(hal_sensor_t *sensor);

/**
 * @brief Register all built-in sensors
 * @return HAL_OK if all registered successfully (or some errors logged)
 */
hal_error_t hal_sensor_register_all(void);

/**
 * @brief Initialize sensor subsystem (register + init all)
 * @return HAL_OK on success, negative error code on failure
 */
hal_error_t hal_sensor_system_init(void);

/**
 * @brief Get sensor instance by type
 * @param type Sensor type
 * @return Pointer to sensor instance or NULL if not found
 */
hal_sensor_t* hal_sensor_get(hal_sensor_type_t type);

/**
 * @brief Initialize all registered sensors
 * @return HAL_OK on success, negative error code on failure
 */
hal_error_t hal_sensor_init_all(void);

#endif /* HAL_SENSOR_H */
