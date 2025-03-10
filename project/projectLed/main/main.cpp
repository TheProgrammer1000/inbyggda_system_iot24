#include <stdio.h>

#include "analog_led.h"
#include "analog_to_digital.h"
#include "binary_led.h"
#include "button.h"
#include "GPIO.h"
#include "led_controller.h"
#include "nvs.h"
#include "printer.h"
#include "esp_sleep.h"



void onPressedButton(int pin) {
    PRINTF_COLOR(ANSI_BLUE, "BUTTON PRESSED PIN: %d" NEW_LINE, pin);
}




extern "C" void app_main(void)
{


    ledc_timer_config_t ledcTimerConf;

    ledcTimerConf.timer_num = LEDC_TIMER_0;
    ledcTimerConf.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimerConf.freq_hz = 4000;
    ledcTimerConf.duty_resolution = LEDC_TIMER_12_BIT;
    ledcTimerConf.clk_cfg = LEDC_AUTO_CLK;
    ledcTimerConf.deconfigure = false;

    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimerConf));

    myAnalogLed::analogLed analogLed1(5, ledcTimerConf.timer_num, LEDC_CHANNEL_2, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE);    
    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE, ADC_CHANNEL_2, ADC_BITWIDTH_12, ADC_ATTEN_DB_12);
    my_button::button button1(4, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_ENABLE, GPIO_INTR_DISABLE);
    
    
    analogLed1.init();
    adc1.init();
    button1.init();

    button1.onPressed_cb = onPressedButton;



    ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup((1UL << button1.get_pin()), ESP_GPIO_WAKEUP_GPIO_HIGH));

    //ESP_ERROR_CHECK(esp_deep_sleep_try_to_start());


    static TickType_t startTick = xTaskGetTickCount();

    while (1)
    {   
        TickType_t elaspedTickSinceStart = xTaskGetTickCount();

        
        if(elaspedTickSinceStart - startTick >= pdMS_TO_TICKS(5000)) {
            
            ESP_ERROR_CHECK(esp_deep_sleep_try_to_start());
            
            PRINTF_COLOR(ANSI_MAGENTA, "HERRE" NEW_LINE);
            startTick = xTaskGetTickCount();
        }

        button1.update();

        int LDRInputResistans = adc1.getVoltageValueFromLDR();
        
        if(LDRInputResistans != -1) {
            analogLed1.setLed(LDRInputResistans);
            analogLed1.update();
        }

        vTaskDelay(pdMS_TO_TICKS(30));
        //analogLed1.update();
    }
    
}