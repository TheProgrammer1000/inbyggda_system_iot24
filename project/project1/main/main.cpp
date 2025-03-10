#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "analog_led.h"
#include "analog_to_digital.h"
#include "binary_led.h"
#include "GPIO.h"
#include "printer.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



extern "C" void app_main(void)
{
    
    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE, ADC_CHANNEL_2, ADC_BITWIDTH_12, ADC_ATTEN_DB_12);
    
    adc1.init();

    while(1) {
        adc1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }

}  