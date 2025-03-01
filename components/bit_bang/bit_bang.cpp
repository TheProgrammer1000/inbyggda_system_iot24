#include "bit_bang.h"


namespace myBitBang {

    /**
     * Skicka data en 1 som indikerar att nu komemr ett meddelande
     * efter denna bit som är 1 kommer meddlande på 4 bitar!
     *  
     * Ha en timer en takt som går en gång i sekunden och lyssnar på 
     * meddalande som kommer in
     * 
     */
    void bitBang::task_func_wrapper(void *param) {
        // Convert the task parameter back to a bitBang pointer
        bitBang* instance = static_cast<bitBang*>(param);
        instance->my_task_func(param);
    }

    void bitBang::my_task_func(void *params) {
        // Now you can access instance members safely

        char bufferChar;
        for (;;)
        {
            if(xQueueReceive(queueHandle, &bufferChar, (TickType_t) 10) == pdTRUE) {
                PRINTF_COLOR(ANSI_BLUE, "Sucessfully recieved: %c" NEW_LINE, bufferChar);
            }
            vTaskDelay(pdMS_TO_TICKS(30));
        }
        
    }

     void bitBang::task_init() {
        // Pass "this" as the task parameter
        xTaskCreate(task_func_wrapper, "myTask", 4096, this, 3, &taskHandle);
        configASSERT(taskHandle);
    }


 
    
    void IRAM_ATTR bitBang::isrCallBackFunc(void* data) {
        bitBang* instance = static_cast<bitBang*>(data);
   
        if (instance->queueHandle != NULL) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            char charachter = 'a';
            xQueueSendFromISR(instance->queueHandle, (void*)&charachter, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }


        
    }
   

    void bitBang::init(int baudRate) {
        this->baudRate = baudRate;
        this->level = 0;
        this->isHighOrLow = false;
        this->taskHandle = NULL;
        this->queueHandle = xQueueCreate(15, sizeof(uint8_t));

        
    }

    void bitBang::sendCommandB() {
        if (queueHandle != NULL) {
            char charachter = 'b';
            xQueueSend(queueHandle, (void*)&charachter, pdMS_TO_TICKS(10));
        }
    }
   

    void bitBang::update() {
        this->tickSinceStart = xTaskGetTickCount(); // Spara starttid
        TickType_t currentTick = xTaskGetTickCount(); // Uppdatera currentTick varje iteration

        
        
        // if(currentTick - tickSinceStart >= pdMS_TO_TICKS(baudRate)) {

        //     //PRINTF_COLOR(ANSI_BLUE, "HERRE" NEW_LINE);
        //     // tickSinceStart = xTaskGetTickCount(); // Återställ tickSinceStart
        //     // pulseCounter = 100;
        //     gpio_set_level((gpio_num_t)gpioTransmitter.getPin(), 0);

        // }
    
    }
}