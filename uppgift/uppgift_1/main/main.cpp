#include "nvs.h"
#include "esp_log.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include <vector>



extern "C" void app_main(void)
{  

    myBinaryLed::binaryLed binaryLeds[] = {
        myBinaryLed::binaryLed(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
        myBinaryLed::binaryLed(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE)
    };

    for (int i = 0; i < 2; i++)
    {
        binaryLeds[i].init();
    }
    
    myAnalogLed::analogLed analogLeds[] = {
        myAnalogLed::analogLed(12, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE),
        myAnalogLed::analogLed(5, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE) 
    };

  
    for (int i = 0; i < 2; i++)
    {
        analogLeds[i].init();
    }
    
    
    //analogLeds[1].sineWave(pdMS_TO_TICKS(1000));
    //myAnalogLed::analogLed analog1(12, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE);

    //analog1.init();

    myLedController::ledController ledcontroller;

    ledcontroller.init(binaryLeds, 2, analogLeds, 2);

    //analog1.sineWave(pdMS_TO_TICKS(2000));
    //analog1.setLed(0);

   

    //ledcontroller.getAnalogLeds()[0].sineWave(2000);

    while(1)
    {
        // for (int i = 0; i < 2; i++)
        // {
        //     analogLeds[i].update();
        // }
        
        // analogLeds[0].update();
        // analogLeds[1].update();
        //analog1.update();
        //ledcontroller.getAnalogLeds()[0].sineWave(2000);
        ledcontroller.blinkAll(1000, 1000);
        // ledcontroller.getBinaryLeds()[1].update();
        // ledcontroller.getBinaryLeds()[0].update();

        vTaskDelay(pdMS_TO_TICKS(30));
    }
    

    


   
    //ledcontroller.init();

    // myNvs::nvs nvs;
    
    // nvs.init();
    //ESP_ERROR_CHECK(nvs_erase_all(nvs.getNvsHandle()));
    //nvs.init();
    // printf("my serialValue: %s\n",nvs.getSerialNumber());
    // printf("my deviceNAme: %s\n",nvs.getDeviceName());

    //nvs.setDeviceName("myDev2");
    //nvs.setSerialNumber("AA:BB:CC:DD22");


    //printf("\nHello from main!\n");
}