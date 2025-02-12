#include <stdio.h>
#include "button.h"


void myFunctionOnRealsed(int pin) {
    printf("Realsed pin: %d\n", pin);        
}

void myFunctionPressed(int pin) {
    printf("Pressed from pin: %d\n", pin);        
}



extern "C" void app_main(void)
{
    my_button::button button1(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_POSEDGE);
    my_button::button button2(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_POSEDGE);
    
    button1.init();
    button2.init();

    button1.setOnPressed(myFunctionPressed);

    button2.setOnPressed(myFunctionPressed);
    //button1.setOnRealesed(myFunctionOnRealsed);

    while (1)
    {
        button1.update(); 
        button2.update();  

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}