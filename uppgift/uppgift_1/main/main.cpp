#include <stdio.h>
#include "binary_led.h"

extern "C" void app_main(void)
{  

    myBinaryLed::binaryLed binaryled1(3, LEDC_CHANNEL_0, 0b11111111, LEDC_INTR_DISABLE);

    binaryled1.init();
    binaryled1.blink(4000, 4000);
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}