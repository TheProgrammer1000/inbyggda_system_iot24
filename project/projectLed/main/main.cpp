#include <stdio.h>

#include "analog_led.h"
#include "analog_to_digital.h"
#include "binary_led.h"
#include "button.h"
#include "GPIO.h"
#include "led_controller.h"
#include "nvs.h"
#include "printer.h"

extern "C" void app_main(void)
{
    PRINTF_COLOR(ANSI_BLUE, "HELLO WORLD!" NEW_LINE);


    ledc_timer_config_t ledcTimerConf;

    ledcTimerConf.timer_num = LEDC_TIMER_0;
    ledcTimerConf.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimerConf.freq_hz = 4000;
    ledcTimerConf.duty_resolution = LEDC_TIMER_12_BIT;
    ledcTimerConf.clk_cfg = LEDC_AUTO_CLK;
    ledcTimerConf.deconfigure = false;

    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimerConf));


    myAnalogLed::analogLed analogLed1(5, ledcTimerConf.timer_num, LEDC_CHANNEL_2, LEDC_TIMER_12_BIT, 0, LEDC_INTR_DISABLE);
    analogLed1.init();

    analogLed1.setLed(2000);

    //analogLed1.sineWave(pdMS_TO_TICKS(2000));

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(30));
        analogLed1.update();
    }
    
}