#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_mac.h"
#include <driver/gpio.h>


namespace my_task {
    void my_task_func(void* pvParameter);
    void task_init();
}

