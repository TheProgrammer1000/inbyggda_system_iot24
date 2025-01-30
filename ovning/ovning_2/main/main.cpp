#include <stdio.h>
#include "printer.h"
#include "helper_component.h"
#include "esp_log.h"



extern "C" void app_main(void)
{
    int a = ovning_2::func();
    ESP_LOGI("MAIN", "got value %d", a);
    a = inbyggda_iot24::func();
    ESP_LOGI("MAIN", "got value %d", a);
}


