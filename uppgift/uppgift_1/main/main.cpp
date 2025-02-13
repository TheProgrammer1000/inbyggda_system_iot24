#include <stdio.h>
#include "button.h"
#include "adc_one.h"



    /*
            typedef void (*onThreshold_t)(int pin, int value);
            onThreshold_t onThreshold_cb;

            void setOnThreshold(int threshold, bool risingEdge, onThreshold_t onThreshHoldFunc);
    
    */




void myFunc(int pin, int value) {
    printf("Hello!!\n");
}



extern "C" void app_main(void)
{  

    
    //my_button::button button1(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);
    //button1.setOnPressed(myFunc);

    adcOneMode::adc myAdc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE);
    
    myAdc1.init();


    //myAdc1.setOnThreshold();



    while (1)
    {
        //button1.update();
        //myAdc1.update();
        myAdc1.update();
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}