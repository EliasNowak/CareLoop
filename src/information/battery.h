/*
 * Battery Monitoring Header
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize battery monitoring
 * 
 * @return 0 on success, negative error code on failure
 */
int battery_init(void);

/**
 * @brief Update battery readings (call periodically)
 */
void battery_update(void);

/**
 * @brief Get battery level percentage
 * 
 * @return Battery level (0-100%)
 */
uint8_t battery_get_level(void);

/**
 * @brief Get battery voltage
 * 
 * @return Battery voltage in millivolts
 */
uint16_t battery_get_voltage(void);

/**
 * @brief Check if battery is low
 * 
 * @return True if battery level is low (<= 20%)
 */
bool battery_is_low(void);

/**
 * @brief Check if battery is critically low
 * 
 * @return True if battery level is critical (<= 5%)
 */
bool battery_is_critical(void);

/**
 * @brief Check if battery is charging
 * 
 * @return True if battery is currently charging
 */
bool battery_is_charging(void);

#endif /* BATTERY_H_ */
