#include <stdio.h>
#include "interrupts.h"


namespace interrupts_esp
{

    void IRAM_ATTR myInterruptHandlerFunc(void* args) {
        // Code that executes quickly
         // Konvertera void*-pekaren tillbaka till en pekare av typen MyInterruptHandlerClass*
        Interrupt_arg* handlerObject = static_cast<Interrupt_arg*>(args);
        handlerObject->interrupt_flag = 1;

        printf("Interrupt Triggered!\n"); // Add this to check if ISR runs
        return;
    }



    void init_interrupt(void* args)
    {    
        esp_err_t esp_err_interrupt = esp_intr_alloc(ETS_INTERNAL_TIMER1_INTR_SOURCE,  ESP_INTR_FLAG_LEVEL1, myInterruptHandlerFunc, args, NULL);

        if(esp_err_interrupt == ESP_ERR_INVALID_ARG) {
            PRINTF_COLOR_WITH_LINE(ANSI_RED, "INVALID PARAMETERS");
        }
        else if(esp_err_interrupt == ESP_ERR_NOT_FOUND) {
            PRINTF_COLOR_WITH_LINE(ANSI_RED, " No free interrupt found with the specified flags");
        }
        else if(esp_err_interrupt == ESP_OK) {
            PRINTF_GROUP_SUCCESFUL("interrupt successful!");
        }
    }
} 


