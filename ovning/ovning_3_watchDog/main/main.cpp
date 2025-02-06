#include <stdio.h>
#include "printer.h"
#include "driver/gpio.h"
#include "watch_dog.h"

#define WATCH_DOG_TIMEOUT 3000
#define WATCH_DOG_CORE 0
#define TASK_RESET_PERIOD 2000


volatile bool gpio_interrupt_triggered = false;

static void IRAM_ATTR gpio_isr_handler_func(void* args) {
    gpio_interrupt_triggered = true;  
}


extern "C" void app_main(void)
{
    gpio_config_t my_gpio_config_3 = {
        .pin_bit_mask = (1UL << 3),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };

    gpio_config_t my_gpio_config_4 = {
        .pin_bit_mask = (1UL << 4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_config(&my_gpio_config_3));
    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_config(&my_gpio_config_4));

    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));

    gpio_isr_handler_add(GPIO_NUM_3, gpio_isr_handler_func, (void*) GPIO_NUM_3);
    gpio_dump_io_configuration(stdout, (1ULL << 3 | 1ULL << 4));


    my_wdt_to_task::wdt_task_init();

    while (1) {


        my_wdt_to_task::wdt_task_feeed_watchdog();
        if(gpio_interrupt_triggered) {
            PRINTF_COLOR(ANSI_BLUE, "INTERRUPT WAS FIRED!" NEW_LINE);
            gpio_interrupt_triggered = false;
        }

        // gpio_set_level(GPIO_NUM_4, 1);
        // vTaskDelay(30 / portTICK_PERIOD_MS);
        // gpio_set_level(GPIO_NUM_4, 0);
        // vTaskDelay(20 / portTICK_PERIOD_MS);
    }

    my_wdt_to_task::wdt_task_delete();
}