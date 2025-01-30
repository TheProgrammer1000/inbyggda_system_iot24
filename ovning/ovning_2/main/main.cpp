#include <stdio.h>
#include "printer.h"
#include "helper_component.h"
//#include "lektion_ovning2.h" 
#include "esp_log.h"



extern "C" void app_main(void)
{
    int a = ovning_2::func();
    ESP_LOGI("MAIN", "got value %d", a);
    a = inbyggda_iot24::func();
    ESP_LOGI("MAIN", "got value %d", a);


    // klar
    //ovning_2::printNumber(1, 10);


    ovning_2::takeTimeAndPrintEverySecond(10000);

    //lektion_ovning2::printNumbers(1, 10);
    
}


