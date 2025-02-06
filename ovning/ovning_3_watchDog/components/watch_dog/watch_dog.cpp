#include <stdio.h>
#include "watch_dog.h"


namespace my_wdt_to_task {
    void wdt_task_init(void)
    {
      esp_task_wdt_config_t wtd_config = {
        .timeout_ms = WATCH_DOG_TIMEOUT,
        .idle_core_mask = WATCH_DOG_CORE,
        .trigger_panic = TRIGGER_PANIC
      };


      ESP_ERROR_CHECK_WITHOUT_ABORT(esp_task_wdt_init(&wtd_config));
      PRINTF_GROUP_SUCCESFUL("Successfully configured watchDog" NEW_LINE);

      
      ESP_ERROR_CHECK_WITHOUT_ABORT(esp_task_wdt_add(NULL));
      PRINTF_GROUP_SUCCESFUL("Successfully added watchDog to task" NEW_LINE);
    }

    void wdt_task_delete(void) {
        ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
        PRINTF_GROUP_SUCCESFUL("Unsubscribed from TWDT\n");
    }

}






    // // behöver sparka på watch_dogen för att den inte ska komma till input
  
    // while (1)
    // {
    //     esp_task_wdt_reset();
    //     vTaskDelay(20);
    // }
    
 