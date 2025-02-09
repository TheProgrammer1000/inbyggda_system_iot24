#include <stdio.h>
#include "driver/ledc.h"
#include "printer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"


#define TIMER_FREQ 4000 // Frequency in Hertz. Set frequency at 4 kHz

#define WAIT_FOR_BUTTON 30



extern "C" void app_main(void)
{
    gpio_config_t gpioConfig;

    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pin_bit_mask = (1UL << 2 | 1UL << 8);
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&gpioConfig));
    
    /*
        Sätt en konstant volt sedan när någon trycker på diverse knappar så kolla i loppen om den är nedstryckt säg första så då öka ljuset.
    */


    ledc_timer_config_t ledcTimerConfig;

    ledcTimerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimerConfig.clk_cfg = LEDC_AUTO_CLK;
    ledcTimerConfig.timer_num = LEDC_TIMER_0;
    ledcTimerConfig.freq_hz = TIMER_FREQ;
    ledcTimerConfig.duty_resolution = LEDC_TIMER_8_BIT;
    ledcTimerConfig.deconfigure = false;
    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimerConfig));
    PRINTF_COLOR(ANSI_MAGENTA, "Successfully configuered ledc timer" NEW_LINE);


    ledc_channel_config_t ledcChannelConfig;

    ledcChannelConfig.gpio_num = 3;
    ledcChannelConfig.channel = LEDC_CHANNEL_0;
    ledcChannelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcChannelConfig.intr_type = LEDC_INTR_DISABLE;
    ledcChannelConfig.timer_sel = LEDC_TIMER_0;
    ledcChannelConfig.sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
    ledcChannelConfig.duty = 0b00000011;
    ledcChannelConfig.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledcChannelConfig));
    PRINTF_COLOR(ANSI_MAGENTA, "Successfully configuered ledc channel config" NEW_LINE);
    PRINTF_COLOR(ANSI_BLUE, "Current duty is: %ld" NEW_LINE, ledc_get_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel));

    int duty = ledcChannelConfig.duty;


    TickType_t startTimer = xTaskGetTickCount();

    while(1) {

        int gpio_2_level = gpio_get_level(GPIO_NUM_2);
        int gpio_8_level = gpio_get_level(GPIO_NUM_8);



        if(gpio_2_level == 1) {
            TickType_t elapsedTimeSinceStart = xTaskGetTickCount() - startTimer;

            if(elapsedTimeSinceStart >= pdMS_TO_TICKS(WAIT_FOR_BUTTON)) {
                startTimer = xTaskGetTickCount();
                PRINTF_COLOR(ANSI_MAGENTA, "GPIO_2_LEVEL IS TRUE!" NEW_LINE);

                duty -= 3;


                if(duty == 0) {
                    continue;
                }
                else if(duty <= 0) {
                    duty = 0;
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel, duty);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel);
                    PRINTF_COLOR(ANSI_GREEN, "IT IS minimum light" NEW_LINE);
                }
                else {
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel, duty);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel);
                }
                PRINTF_COLOR(ANSI_BLUE, "Current duty is: %ld" NEW_LINE, ledc_get_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel));            
            }
        }

        // increase light
        if(gpio_8_level == 1) {
            TickType_t elapsedTimeSinceStart = xTaskGetTickCount() - startTimer;

            if(elapsedTimeSinceStart >= pdMS_TO_TICKS(WAIT_FOR_BUTTON)) {
                startTimer = xTaskGetTickCount();
                PRINTF_COLOR(ANSI_MAGENTA, "GPIO_8_LEVEL IS TRUE!" NEW_LINE);    

                duty += 3;

                if(duty >= 256) {
                    duty = 256;
                    PRINTF_COLOR(ANSI_GREEN, "IT IS maximum light" NEW_LINE);
                }
                else {
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel, duty);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel);
                }
                PRINTF_COLOR(ANSI_BLUE, "Current duty is: %ld" NEW_LINE, ledc_get_duty(LEDC_LOW_SPEED_MODE, ledcChannelConfig.channel));            
            }
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}