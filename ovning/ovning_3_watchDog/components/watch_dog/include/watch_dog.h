#pragma once

#include "printer.h"
#include "esp_task_wdt.h"

#define WATCH_DOG_TIMEOUT 3000
#define WATCH_DOG_CORE 0
#define TRIGGER_PANIC true



namespace my_wdt_to_task { 

    /*
        @brief
        configure watchdog task default to core 0
        timeout 3000 ms and trigger_panic is true
    */
    void wdt_task_init(void);
    

   /*
        @brief
        Feeding the watchdog 
    */
    void inline wdt_task_feeed_watchdog(void) {
        esp_task_wdt_reset();
    }

    /*
        @brief
        Deleting the current watchdog timer the
        task is currently added to
    */
    void wdt_task_delete(void);
}

