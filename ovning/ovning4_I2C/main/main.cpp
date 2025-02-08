#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c_master.h"


#define GLITCH_IGNORE_COUNT 7


extern "C" void app_main(void)
{
    i2c_master_bus_handle_t masterBus_handler;
    i2c_master_bus_config_t i2cmaster_bussConfig;

    i2cmaster_bussConfig.i2c_port = - 1;
    i2cmaster_bussConfig.sda_io_num = GPIO_NUM_6;
    i2cmaster_bussConfig.scl_io_num = GPIO_NUM_7;
    i2cmaster_bussConfig.clk_source = I2C_CLK_SRC_DEFAULT;
    i2cmaster_bussConfig.glitch_ignore_cnt = GLITCH_IGNORE_COUNT;
    i2cmaster_bussConfig.intr_priority = 0;


    i2c_device_config_t i2cDeviceConfig;

    i2cDeviceConfig.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    i2cDeviceConfig.device_address = 0x58;


    ESP_ERROR_CHECK(i2c_new_master_bus(&i2cmaster_bussConfig, &masterBus_handler));



}