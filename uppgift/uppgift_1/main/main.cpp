#include <stdio.h>
#include "button.h"

extern "C" void app_main(void)
{
    my_button::button button1(GPIO_NUM_3, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_POSEDGE);
    
    
    button1.init(button1.pin_bit_mask, button1.mode, button1.pull_up_en, button1.pull_down_en, button1.intr_type);


    while (1)
    {
        button1.update();

        if(button1.buttonPressed == true) {
            printf("Knappen nedtryck!!!");
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}