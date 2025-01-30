#include <stdio.h>
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


namespace ovning_2 {
    int func(void)
    {
        return CONFIG_PRINT_LEVEL;
    }

    void printNumber(int start, int stop) {
        for (int i = start; i <= stop; i++)
        {
            printf("%d\n", i);
        }   
    }

    int multiply(int a, int b) {
        return a * b;
    }

    void takeTimeAndPrintEverySecond(int time) {
    
        int timeElapsed = 0;
        do
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("Time elapsed %d\n", timeElapsed);
            timeElapsed += 1000;

        } while (timeElapsed < time);
    }
}

