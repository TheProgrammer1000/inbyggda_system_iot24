#include <stdio.h>
#include "esp_task_wdt.h"

#define WATCH_DOG_TIMEOUT 3000
#define WATCH_DOG_CORE 0



extern "C" void app_main(void)
{
    esp_task_wdt_config_t wtd_config = {
        .timeout_ms = WATCH_DOG_TIMEOUT,
        .idle_core_mask = WATCH_DOG_CORE,
        .trigger_panic = false
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_task_wdt_init(&wtd_config));

    esp_task_wdt_add(NULL);

    // behöver sparka på watch_dogen för att den inte ska komma till input
    while (1)
    {
        ;;
    }
    
        
        //esp_task_wdt_add(NULL);

      
        

        //esp_task_wdt_delete(NULL);
}