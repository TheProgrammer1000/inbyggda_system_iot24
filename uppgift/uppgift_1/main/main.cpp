#pragma once
#include <stdio.h>
#include "binary_led.h"
#include "analog_led.h"


void myFunctionToAdc() {
    printf("From potentimeter!!" NEW_LINE);
}


extern "C" void app_main(void)
{  

    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE);

    adc1.init();
    adc1.setOnThreshold(2000, false, myFunctionToAdc);


    //myBinaryLed::binaryLed binaryLed1(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);

    //binaryLed1.init();
    //binaryLed1.blink(3000, 3000);
    //binaryLed1.setLed(myFunctionToAdc);
    
    

    
    while(1) {
        //binaryLed1.update();
        adc1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}