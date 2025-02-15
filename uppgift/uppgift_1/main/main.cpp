#include <stdio.h>
#include "binary_led.h"


/*

      ledc_channel_config_t ledc_channel_conf;
            ledc_channel_conf.gpio_num =            3;
            ledc_channel_conf.speed_mode =          LEDC_LOW_SPEED_MODE;
            ledc_channel_conf.channel =             LEDC_CHANNEL_0;
            ledc_channel_conf.duty    =             0b1111111111;  // default setting this to 0 and already assign user value
            ledc_channel_conf.hpoint  =             0;
            ledc_channel_conf.flags.output_invert = 0;
            ledc_channel_conf.intr_type =           LEDC_INTR_DISABLE;
            ledc_channel_conf.timer_sel =           ledc_timer.timer_num;
            ledc_channel_conf.sleep_mode =          LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
            

*/


extern "C" void app_main(void)
{  

    myBinaryLed::binaryLed binaryled1(3, LEDC_CHANNEL_0, 0b11111111, LEDC_INTR_DISABLE);

    binaryled1.init();
    binaryled1.blink(4000, 4000);
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    //printf("HEllo from main \n");
    

}