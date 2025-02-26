#pragma once

#include "printer.h"
#include <stdio.h>
#include "driver/gpio.h"

namespace myGpio {
    class Gpio {
        private:
            int pin;                     
            gpio_mode_t mode;            
            gpio_pullup_t pullUpEnable;  
            gpio_pulldown_t pullDownEnable;
            gpio_int_type_t interuptType; 

            
        public:
            Gpio(int pin, gpio_mode_t mode, gpio_pullup_t pullUpEnable, gpio_pulldown_t pullDownEnable, gpio_int_type_t interuptType);
            void init();
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