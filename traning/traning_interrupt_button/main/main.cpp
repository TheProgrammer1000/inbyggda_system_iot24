#include <stdio.h>
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"

#define MY_INTERRUPT_PIN GPIO_NUM_3
#define WDT_TIMOUT_MS 3000

volatile static int buttonIsPressed = false;

void IRAM_ATTR gpio_isr_handler_func(void *arg) {
    buttonIsPressed = true;
}

extern "C" void app_main(void)
{
    printf("Hello world!\n");
    PRINTF_COLOR(ANSI_BLUE, "Hello World!" NEW_LINE);
    PRINTF_COLOR(ANSI_GREEN, "HELLLO AGAIN" NEW_LINE);

    gpio_config_t gpioConfig;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = (1UL << 3);
    gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE; 
    gpioConfig.intr_type = GPIO_INTR_POSEDGE;

    gpio_config(&gpioConfig);

    
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(gpio_isr_handler_add(MY_INTERRUPT_PIN, gpio_isr_handler_func, (void*) MY_INTERRUPT_PIN));

    esp_task_wdt_config_t watchDogConfig;
    watchDogConfig.idle_core_mask = 0x1;
    watchDogConfig.timeout_ms = WDT_TIMOUT_MS;
    watchDogConfig.trigger_panic = false;

    esp_task_wdt_init(&watchDogConfig);
    esp_task_wdt_add(NULL);


    TickType_t startTick = xTaskGetTickCount();

    int level;

    while (1)
    {
        level = gpio_get_level(MY_INTERRUPT_PIN);

        if(buttonIsPressed == true && level == 0) {             // Här får vi precis när interrupten har skett och level hunnit gå ner till 0 och då vet vi för buttonIspressed är true att knappen varit nedtryckt så får vi inge debounce effekt!
            TickType_t elapsedTimeFromTickStart = xTaskGetTickCount() - startTick;

            if(elapsedTimeFromTickStart > pdMS_TO_TICKS(30)) {
                PRINTF_COLOR(ANSI_BLUE, "BUTTON IS PRESSED!" NEW_LINE);
                buttonIsPressed = false;
                esp_task_wdt_reset();

                startTick = xTaskGetTickCount();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }

    esp_task_wdt_delete(NULL);
}