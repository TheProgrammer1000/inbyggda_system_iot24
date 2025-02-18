#pragma once
#include <stdio.h>
#include "binary_led.h"
#include "analog_led.h"


void myFunctionToAdc(char* msg) {
    printf("%s" NEW_LINE, msg);
}


extern "C" void app_main(void)
{  

    myAnalogLed::analogLed analogLed1(3, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE);
    analogLed1.init();
    analogLed1.setLed(myFunctionToAdc);
    
    // adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE);

    // adc1.init();
    // adc1.setOnThreshold(2000, false, myFunctionToAdc);


    //myBinaryLed::binaryLed binaryLed1(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);

    //binaryLed1.init();
    //binaryLed1.blink(3000, 3000);
    //binaryLed1.setLed(myFunctionToAdc);
    
    

    
    while(1) {
        //binaryLed1.update();
        //adc1.update();
        analogLed1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}