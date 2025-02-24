#include "nvs.h"
#include "esp_log.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"

extern "C" void app_main(void)
{  

    // pin 12, 5, 2, 3 works

    myBinaryLed::binaryLed binaryLeds[] = {
        myBinaryLed::binaryLed(6, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
        myBinaryLed::binaryLed(5, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE)
    };
    
    myAnalogLed::analogLed analogLeds[] = {
        myAnalogLed::analogLed(2, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE),
        myAnalogLed::analogLed(3, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE)
    };


    for (int i = 0; i < 2; i++)
    {
        analogLeds[i].init();
        //analogLeds[i].sineWave(pdMS_TO_TICKS(2000));
        //analogLeds[i].setLed(500);

        binaryLeds[i].init();
        //binaryLeds[i].setLed(1);

    }

    
    myLedController::ledController ledcontroller;
    ledcontroller.init(binaryLeds, 2, analogLeds, 2);

    while(1)
    {
        // for (int i = 0; i < 2; i++)
        // {
        //     analogLeds[i].update();
        //     binaryLeds[i].update();
        // }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}