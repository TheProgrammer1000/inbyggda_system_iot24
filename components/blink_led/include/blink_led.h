

/**
 * When a timer is no longer needed by any channel, it can be deconfigured by calling the same function ledc_timer_config(). The configuration structure ledc_timer_config_t passes in should be:

    ledc_timer_config_t::speed_mode The speed mode of the timer which wants to be deconfigured belongs to (ledc_mode_t)

    ledc_timer_config_t::timer_num The ID of the timers which wants to be deconfigured (ledc_timer_t)

    ledc_timer_config_t::deconfigure Set this to true so that the timer specified can be deconfigured 
 * 
 */
#pragma once

#include <stdio.h>
#include "esp_mac.h"
#include "driver/ledc.h"
#include "esp_log.h"
//#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


namespace blink_led {
    void func(void);
    void ledc_init();
}
