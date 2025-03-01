#pragma once
#include "GPIO.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_mac.h"
#include <FreeRTOSConfig.h>
#include "driver/dedic_gpio.h"

namespace myBitBang {
    class bitBang {
        private:
            myGpio::Gpio gpioTransmitter;
            myGpio::Gpio gpioReciever;
            int baudRate;
            int level;
            bool isHighOrLow;
            TickType_t tickSinceStart;
            int dataBuffer[256];
            QueueHandle_t queueHandle;

            TaskHandle_t taskHandle;

             // The static wrapper function that conforms to the required signature
            static void task_func_wrapper(void *param);
            static void bitBang_Task(void* arg);
            void my_task_func(void* pvParameter);

        


        public:
            void sendCommandB();
    
            void task_init();

            bitBang(myGpio::Gpio& gpioTransmitter, myGpio::Gpio& gpioReciever) : gpioTransmitter(gpioTransmitter), gpioReciever(gpioReciever) {}
            void init(int setBaudRate);

      

                
            /**
            * @brief The data parameter will contain a object of gpio class
            * 
            */
            static void IRAM_ATTR isrCallBackFunc(void* data); // Correct declaration

            const myGpio::Gpio& getGpioTransmitter() const {
                return gpioTransmitter;
            }

            const myGpio::Gpio& getGpioReciever() const {
                return gpioReciever;
            }
    
            // void setGpioTransmitter(const myGpio::Gpio& newGpioTransmitter) {
            //     gpioTransmitter = newGpioTransmitter;
            // }
        
            // void setGpioReciever(const myGpio::Gpio& newGpioReciever) {
            //     gpioReciever = newGpioReciever;
            // }

            void update();
    };
}
