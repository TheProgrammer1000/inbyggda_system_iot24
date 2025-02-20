#include <stdio.h>
#include <esp_sleep.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "binary_led.h"
#include "printer.h"
#include "button.h"

#define RTC_PIN GPIO_NUM_3
/*
Bonus:
Gå I sleep mode och vakna med knapptryck samtidigt som du har en lampa som ska gå från av till på eller på till av en gang I sekunden
Du får inte missa att blinka oavsett hur mycket du trycker på knappen
*/

void buttonFunc(int pin) {
    printf("Button is pressed!: %d\n", pin);
}

extern "C" void app_main(void)
{
    myBinaryLed::binaryLed binaryLed1(2, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);
    binaryLed1.init();
    binaryLed1.blink(1000, 1000);

    my_button::button button1(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_ENABLE, GPIO_INTR_DISABLE);
    button1.init();

    button1.setOnPressed(buttonFunc);
   
  

    //unsigned int secound = 1000000; // one secound
   

    //int count = 0;

   
    if(esp_sleep_is_valid_wakeup_gpio((gpio_num_t) button1.get_pin())) {
        ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup((1UL << button1.get_pin()), ESP_EXT1_WAKEUP_ANY_HIGH));

        ESP_ERROR_CHECK(esp_light_sleep_start());

        PRINTF_COLOR(ANSI_BLUE, "Success!" NEW_LINE);
    }




    //esp_sleep_enable_ext1_wakeup();

    //esp_sleep_wakeup_cause_t wakeType = esp_sleep_get_wakeup_cause();

    //printf("Wakeup caused by timer Interrupt\n");
    //ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(wakeType));

    while(1) {
        //printf("number: %d\n", count);
        //count++;

        binaryLed1.update();
        button1.update();
      

        //if(count == 10) {
            //esp_deep_sleep(3000000);
            //esp_sleep_wakeup_cause_t wakeType = esp_sleep_get_wakeup_cause();
            //ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(wakeType));
        //}

        vTaskDelay(pdMS_TO_TICKS(50));
        
    } 
}