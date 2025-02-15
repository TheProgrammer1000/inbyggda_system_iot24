#pragma once
#include <stdio.h>
#include "binary_led.h"
#include "analog_led.h"

extern "C" void app_main(void)
{  


    //myAnalogLed::analogLed analogLed1;

    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE);
    adc1.init();



    myBinaryLed::binaryLed binaryled1(3, LEDC_CHANNEL_0,LEDC_TIMER_12_BIT ,0b11111111, LEDC_INTR_DISABLE);

    binaryled1.init();
    binaryled1.blink(4000, 4000);
    





    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}