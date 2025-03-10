#include "esp_mac.h"
#include "nvs.h"
//#include "esp_log.h"
#include "analog_to_digital.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"
#include "GPIO.h"


extern "C" void app_main(void)
{  

    // pin 12, 5, 2, 3 works
    int analogSize = 5;
    
    int binarySize1 = 5;
    int binarySize2 = 5;



    ledc_timer_config_t timerConf;

    timerConf.deconfigure = false;
    timerConf.clk_cfg = LEDC_AUTO_CLK;
    timerConf.duty_resolution = LEDC_TIMER_12_BIT;
    timerConf.freq_hz = 4000;
    timerConf.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConf.timer_num = LEDC_TIMER_0;

    ledc_timer_config(&timerConf);

    //myAnalogLed::analogLed analogLed1(5, timerConf.timer_num, LEDC_CHANNEL_0, timerConf.duty_resolution, 0b111111111111, LEDC_INTR_DISABLE);


    // myBinaryLed::binaryLed binaryLeds1[] = {
    //     myBinaryLed::binaryLed(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(4, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(5, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(6, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE)
    // };


    // myBinaryLed::binaryLed binaryLeds2[] = {
    //     myBinaryLed::binaryLed(0, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(1, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(7, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(10, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
    //     myBinaryLed::binaryLed(11, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE)
    // };
    
    myAnalogLed::analogLed analogLeds[] = {
        myAnalogLed::analogLed(2, timerConf.timer_num, LEDC_CHANNEL_0, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE),
        myAnalogLed::analogLed(3, timerConf.timer_num, LEDC_CHANNEL_1, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE),
        myAnalogLed::analogLed(4, timerConf.timer_num, LEDC_CHANNEL_2, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE),
        myAnalogLed::analogLed(5, timerConf.timer_num, LEDC_CHANNEL_3, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE),
        myAnalogLed::analogLed(6, timerConf.timer_num, LEDC_CHANNEL_4, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE)
    };



    for (int i = 0; i < analogSize; i++)
    {
        analogLeds[i].init();
        
        analogLeds[i].sineWave(pdMS_TO_TICKS(2000));
    }


    // for (int i = 0; i < binarySize1; i++)
    // {
    //     binaryLeds1[i].init();

    //     binaryLeds1->setLed(1);
    // }

    // for (int i = 0; i < binarySize2; i++)
    // {
    //     binaryLeds2[i].init();
    //     binaryLeds1[i].init();
    // }

    
    myLedController::ledController ledcontroller1;
    // myLedController::ledController ledcontroller2;

    ledcontroller1.init(0, 0, analogLeds, analogSize);
    // //ledcontroller1.init(binaryLeds1, binarySize1, 0, 0);
    // ledcontroller1.init(binaryLeds2, binarySize2, 0, 0);

    
        

    while(1)
    {
        for (int i = 0; i < analogSize; i++)
        {
            analogLeds[i].update();
        }
        
        vTaskDelay(pdMS_TO_TICKS(30));
       
    //     //ledcontroller1.blinkAll(2000, 2000);
    //     //ledcontroller1.myLedAnimation(2000, binarySize1, false);
    //     //ledcontroller1.snakeFushionAnimation(2, 500, 5);
    //     // 

    //     //ledcontroller1.snakeAnimation(2, 500, binaryLeds1, binarySize1, false);
    //    //vTaskDelay(pdMS_TO_TICKS(100));
    //    ledcontroller2.snakeAnimation(3, 150, binarySize1, true);
    //    ledcontroller1.snakeAnimation(3, 150, binarySize2, false);
    // vTaskDelay(pdMS_TO_TICKS(30));
    }

}
