#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "driver/i2c_types.h"
#include "printer.h"


#define GLITCH_IGNORE_COUNT 7
#define I2C_SOURCE_CLOCK_SPEED_HZ 100000      // 100kHz
#define I2C_DEVICE_ADDRESS 0x58

extern "C" void app_main(void)
{
    i2c_master_bus_handle_t masterBus_handler;
    i2c_master_bus_config_t i2cMasterBussConfig;
    i2cMasterBussConfig.i2c_port = - 1;
    i2cMasterBussConfig.sda_io_num = GPIO_NUM_6;
    i2cMasterBussConfig.scl_io_num = GPIO_NUM_7;
    i2cMasterBussConfig.clk_source = I2C_CLK_SRC_DEFAULT;
    i2cMasterBussConfig.glitch_ignore_cnt = GLITCH_IGNORE_COUNT;
    i2cMasterBussConfig.intr_priority = 0;
    i2cMasterBussConfig.trans_queue_depth = 5;


    ESP_ERROR_CHECK(i2c_new_master_bus(&i2cMasterBussConfig, &masterBus_handler));


    i2c_device_config_t i2cDeviceConfig;
    i2c_master_dev_handle_t masterBus_deviceHandler;
    i2cDeviceConfig.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    i2cDeviceConfig.device_address = I2C_DEVICE_ADDRESS;
    i2cDeviceConfig.scl_speed_hz = I2C_SOURCE_CLOCK_SPEED_HZ;
    i2cDeviceConfig.scl_wait_us = 0;


    ESP_ERROR_CHECK(i2c_master_bus_add_device(masterBus_handler, &i2cDeviceConfig, &masterBus_deviceHandler));
    PRINTF_COLOR(ANSI_MAGENTA, "Successfully configuered master buss!" NEW_LINE);


    // i2c_slave_config_t slaveConfig;
    // i2c_slave_dev_handle_t slaveDeviceHandler;
    // slaveConfig.i2c_port = - 1;
    // slaveConfig.sda_io_num = GPIO_NUM_6;
    // slaveConfig.scl_io_num = GPIO_NUM_7;
    // slaveConfig.clk_source = I2C_CLK_SRC_DEFAULT;
    // slaveConfig.slave_addr = 0x58,
    // slaveConfig.intr_priority = 0;
    // slaveConfig.addr_bit_len = I2C_ADDR_BIT_LEN_7;
    // slaveConfig.send_buf_depth = 256;

    //ESP_ERROR_CHECK(i2c_new_slave_device(&slaveConfig, &slaveDeviceHandler));
    //PRINTF_COLOR(ANSI_MAGENTA, "Successfully configuered master and slave!");
    




}