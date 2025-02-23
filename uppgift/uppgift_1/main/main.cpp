#include "nvs.h"
#include "esp_log.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"

extern "C" void app_main(void)
{  

    // pin 12, 5, 2, 3 works
    //int analogSize = 2;
    int binarySize = 5;

    myBinaryLed::binaryLed binaryLeds[] = {
        myBinaryLed::binaryLed(6, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
        myBinaryLed::binaryLed(5, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
        myBinaryLed::binaryLed(4, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
        myBinaryLed::binaryLed(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
        myBinaryLed::binaryLed(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE)
    };
    
    // myAnalogLed::analogLed analogLeds[] = {
    //     myAnalogLed::analogLed(2, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE),
    //     myAnalogLed::analogLed(3, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE),
    //     //myAnalogLed::analogLed(4, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE)
    // };


    // for (int i = 0; i < analogSize; i++)
    // {
    //     analogLeds[i].init();
    // }


    for (int i = 0; i < binarySize; i++)
    {
        binaryLeds[i].init();
    }

    
    myLedController::ledController ledcontroller;
    ledcontroller.init(binaryLeds, binarySize, 0, 0);

    while(1)
    {
       
        //ledcontroller.blinkAll(2000, 2000);
        ledcontroller.myLedAnimation(500, binarySize, false);
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}
