#pragma once

#include "esp_rom_sys.h"

#include "esp_attr.h"
#include "printer.h"
#include <stdio.h>
#include "driver/gpio_filter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

namespace myGpio {
    class Gpio {
        private:
            int pin;                     
            gpio_mode_t mode;            
            gpio_pullup_t pullUpEnable;  
            gpio_pulldown_t pullDownEnable;
            gpio_int_type_t interuptType; 

            gpio_isr_t isrHandler;
            void* IsrArg;          
            volatile bool gpioInterruptTriggered;

            bool isrServiceInstalled;
            gpio_glitch_filter_handle_t glitchFilterHandle;

            static void IRAM_ATTR isrCallBackFunc(void* data); // Correct declaration

            
        public:
            Gpio(int pin, gpio_mode_t mode, gpio_pullup_t pullUpEnable, gpio_pulldown_t pullDownEnable, gpio_int_type_t interuptType);
            void init();


            /**
            * @note Installing isr service and adding the specific pin to the interrupt
            * @attention Setting the clock source to default on the glitch filter
            * 
            */
            void attachInterrupt();
            
            /**
            * @note unInstalling isr service and detach the specific pin to the interrupt
            * @attention Also deleting the glitch filter
            */
            void detachInterruptToPin();
            void update();

             // Setter functions
            void setPin(int p) { pin = p; }
            void setMode(gpio_mode_t m) { mode = m; }
            void setPullUpEnable(gpio_pullup_t pullUp) { pullUpEnable = pullUp; }
            void setPullDownEnable(gpio_pulldown_t pullDown) { pullDownEnable = pullDown; }
            void setInterruptType(gpio_int_type_t intType) { interuptType = intType; }

            // Getter functions
            int getPin() const { return pin; }
            gpio_mode_t getMode() const { return mode; }
            gpio_pullup_t getPullUpEnable() const { return pullUpEnable; }
            gpio_pulldown_t getPullDownEnable() const { return pullDownEnable; }
            gpio_int_type_t getInterruptType() const { return interuptType; }

    };
}