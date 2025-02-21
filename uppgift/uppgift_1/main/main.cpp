#include "nvs.h"
#include "esp_log.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"



extern "C" void app_main(void)
{  

    //myBinaryLed::binaryLed binaryLed1(8, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE); // works
    //myBinaryLed::binaryLed binaryLed2(5, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);
    
    //binaryLed1.init();
    //binaryLed2.init();
    

    //binaryLed1.setLed(1);
    //binaryLed2.setLed(1);


    myAnalogLed::analogLed analogLed1(2, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE); // works
    myAnalogLed::analogLed analogLed2(3, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE); // works
    myAnalogLed::analogLed analogLed3(12, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE); // works
    myAnalogLed::analogLed analogLed4(13, LEDC_CHANNEL_0, LEDC_TIMER_10_BIT, 0b1111111111, LEDC_INTR_DISABLE); // works

    analogLed1.init();
    analogLed2.init();
    analogLed3.init();
    analogLed4.init();

    analogLed1.setLed(1);
    analogLed2.setLed(1);
    analogLed3.setLed(1);
    analogLed4.setLed(1);


    myLedController::ledController ledcontroller;


   
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