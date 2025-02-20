#include <stdio.h>
#include "myTask.h"
#include "driver/ledc.h"


namespace my_task {
    void task_init() {

        TaskFunction_t taskFuncPointer = my_task::my_task_func;

        TaskHandle_t xHandle = NULL;
        xTaskCreate(taskFuncPointer, "myTask", 4096, NULL, 3, &xHandle);

        configASSERT(xHandle);
    }

    void my_task_func(void* pvParameter) {
        char* ourTaskName = pcTaskGetName(NULL);
        ESP_LOGI(ourTaskName, "Starting my task!");

        while(1)
        {
            vTaskDelay(pdMS_TO_TICKS(40));
            ESP_LOGI("myTask","Task is here!"); 
        }
    }
}


