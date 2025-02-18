#include <stdio.h>
#include <esp_sleep.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "binary_led.h"
#include "printer.h"

/*
Bonus:
Gå I sleep mode och vakna med knapptryck samtidigt som du har en lampa som ska gå från av till på eller på till av en gang I sekunden
Du får inte missa att blinka oavsett hur mycket du trycker på knappen
*/

extern "C" void app_main(void)
{
    myBinaryLed::binaryLed binaryLed1(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);
    binaryLed1.init();
    binaryLed1.blink(1000, 1000);

    //unsigned int secound = 1000000; // one secound
   

    //int count = 0;

   

    //esp_sleep_wakeup_cause_t wakeType = esp_sleep_get_wakeup_cause();

    //printf("Wakeup caused by timer Interrupt\n");
    //ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(wakeType));

    while(1) {
        //printf("number: %d\n", count);
        //count++;

        binaryLed1.update();
        //ESP_ERROR_CHECK(esp_light_sleep_start());

        //if(count == 10) {
            //esp_deep_sleep(3000000);
            //esp_sleep_wakeup_cause_t wakeType = esp_sleep_get_wakeup_cause();
            //ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(wakeType));
        //}

        vTaskDelay(pdMS_TO_TICKS(50));
        
    } 
}