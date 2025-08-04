/*
 * CareLoop - Health Monitoring Device (Basic Implementation)
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define DEVICE_NAME "CareLoop-Monitor"
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* BLE Advertisement data */
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

/* BLE connection callback */
static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    
    if (err) {
        LOG_ERR("Connection failed (err 0x%02x)", err);
        return;
    }
    
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    LOG_INF("Connected %s", addr);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    char addr[BT_ADDR_LE_STR_LEN];
    
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    LOG_INF("Disconnected from %s (reason 0x%02x)", addr, reason);
    
    /* Restart advertising */
    int err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err);
    }
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
};

/* Simple heart rate simulation */
static uint16_t simulate_heart_rate(void)
{
    static uint16_t hr = 72;
    static int8_t delta = 1;
    
    hr += delta;
    if (hr >= 85) {
        delta = -1;
    } else if (hr <= 65) {
        delta = 1;
    }
    
    return hr;
}

int main(void)
{
    int err;
    uint32_t loop_count = 0;
    
    LOG_INF("=== CareLoop Health Monitor Starting ===");
    LOG_INF("Version: 1.0.0");
    LOG_INF("Build: %s %s", __DATE__, __TIME__);
    
    /* Initialize Bluetooth */
    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err);
        return err;
    }
    
    LOG_INF("Bluetooth initialized");
    
    /* Start advertising */
    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err);
        return err;
    }
    
    LOG_INF("BLE advertising started as '%s'", DEVICE_NAME);
    LOG_INF("CareLoop initialized successfully!");
    
    /* Main application loop */
    while (1) {
        loop_count++;
        
        /* Simulate sensor readings every 5 seconds */
        if (loop_count % 50 == 0) {
            uint16_t hr = simulate_heart_rate();
            LOG_INF("Heart Rate: %d BPM (simulated)", hr);
        }
        
        /* Log status every 10 seconds */
        if (loop_count % 100 == 0) {
            LOG_INF("System running - Loop: %d, Uptime: %d sec", 
                    loop_count, k_uptime_get_32() / 1000);
        }
        
        /* Sleep for 100ms */
        k_msleep(100);
    }
    
    return 0;
}
