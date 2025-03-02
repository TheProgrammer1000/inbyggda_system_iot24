#include "esp_mac.h"
//#include "nvs.h"
//#include "esp_log.h"
#include "analog_to_digital.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"
#include "GPIO.h"


extern "C" void app_main(void)
{  
    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE, ADC_CHANNEL_2, ADC_BITWIDTH_12, ADC_ATTEN_DB_12);
    adc1.init();

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 4000, 
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    myAnalogLed::analogLed analogLed1(5, ledc_timer.timer_num , LEDC_CHANNEL_0, LEDC_TIMER_13_BIT, 0b1111111111111, LEDC_INTR_DISABLE);
    analogLed1.init();
    //analogLed1.sineWave(pdMS_TO_TICKS(2000));


    

    while (1)
    {
        PRINTF_COLOR(ANSI_BLUE, "voltage value: %d" NEW_LINE, adc1.getVoltageValueFromLDR());
        analogLed1.setLed(adc1.getVoltageValueFromLDR());
        analogLed1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    
}
