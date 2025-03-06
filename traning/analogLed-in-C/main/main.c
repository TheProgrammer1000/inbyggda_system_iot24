#include "esp_mac.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

typedef struct {
    ledc_channel_t channel;
    ledc_timer_t timer;
    int pin;
    int value;
    int period; // period in ticks (e.g. pdMS_TO_TICKS(2000))
} led_t;

// Computes the LED duty based on a sine wave, using led->period.
uint32_t led_sin(int period,led_t *led) {
    static TickType_t startTick = 0;
    if (startTick == 0) {
        startTick = xTaskGetTickCount();
    }
    
    // Get the current tick count and compute elapsed ticks.
    TickType_t now = xTaskGetTickCount();
    TickType_t elapsed = now - startTick;
    
    // Calculate phase as a fraction [0, 1) of the period.
    double phase = fmod((double)elapsed, (double)period) / (double)period;
    
    // Convert phase to an angle (0 to 2π).
    double angle = phase * 2.0 * M_PI;
    
    // Compute the sine value (range: -1 to 1).
    double sineValue = sin(angle);
    
    // Map sine value from [-1, 1] to [0, 4096].
    uint32_t duty = (uint32_t) round(((sineValue + 1.0) / 2.0) * 4096.0);
    
    return duty;
}

// Updates the LED duty by computing the sine value and applying it via LEDC.
void update(led_t* led) {
    uint32_t dutyValue = led_sin(pdMS_TO_TICKS(2000), led);
    // // Set the duty for the channel stored in the struct.
    ledc_set_duty(LEDC_LOW_SPEED_MODE, led->channel, dutyValue);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, led->channel);
}

void app_main(void)
{  
    // Configure LEDC timer.
    ledc_timer_config_t ledcTimerConf = {
        .clk_cfg = LEDC_AUTO_CLK,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .freq_hz = 4000,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .deconfigure = false
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimerConf));

    // Configure LEDC channel.
    ledc_channel_config_t ledcChannelConf = {
        .channel    = LEDC_CHANNEL_0,
        .gpio_num   = 3,
        .hpoint     = 0,
        .duty       = 0, // start with duty 0
        .intr_type  = LEDC_INTR_DISABLE,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel  = LEDC_TIMER_0,
        // .flags can be set if needed
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledcChannelConf));

    // Initialize the led_t struct.
    led_t led1 = {
        .channel = LEDC_CHANNEL_0,
        .timer = LEDC_TIMER_0,
        .pin = 3,
        .period = pdMS_TO_TICKS(2000)
    };

    // Main loop: update the LED every 30ms.
    //led_sin(3000, &led1);

    while(1) {
        update(&led1);
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}
