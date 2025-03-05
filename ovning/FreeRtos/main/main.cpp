#include <stdio.h>
#include "analog_led.h"
#include "analog_to_digital.h"
#include "binary_led.h"
#include "GPIO.h"
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void my_task_func(void* pvParameter) {
    char* ourTaskName = pcTaskGetName(NULL);
    ESP_LOGI(ourTaskName, "Starting my task!");

    myBinaryLed::binaryLed binaryLed1(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);

    binaryLed1.init();
    binaryLed1.blink(2000, 2000);

    while(1)
    {
        binaryLed1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}


void task_init() {

    TaskFunction_t taskFuncPointer = my_task_func;

    TaskHandle_t xHandle = NULL;
    xTaskCreate(taskFuncPointer, "myTask", 4096, NULL, 3, &xHandle);

    configASSERT(xHandle);

    // Use the handle to delete the task.
    if(xHandle != NULL)
    {
        vTaskDelete(xHandle);
    }

}

extern "C" void app_main(void)
{
    task_init();
}