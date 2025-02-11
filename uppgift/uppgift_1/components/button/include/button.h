/*
    xxx init( pin, xxx )
        Ska gå att ha flera knappar samtidigt, gäller för alla komponenter
        För cpp får konstruktorn anvöndas för parametrar, men inte för att faktiskt “sätta upp/confugurera esp-idf”

*/

#pragma once
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace my_button {
    class button {
        private:
         

        public:
            button(uint64_t pin_bit_mask, gpio_mode_t mode, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type);
            
            uint64_t pin_bit_mask;          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
            gpio_mode_t mode;               /*!< GPIO mode: set input/output mode                     */
            gpio_pullup_t pull_up_en;       /*!< GPIO pull-up                                         */
            gpio_pulldown_t pull_down_en;   /*!< GPIO pull-down                                       */
            gpio_int_type_t intr_type;      /*!< GPIO interrupt type         */ 

            bool buttonPressed;
            TickType_t startTickButton;

            void init(uint64_t pin, gpio_mode_t mode, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type);
            bool isPressed();
            void setOnPressed(void(*onPressed)(int pin));
            void setupInterrupt();
            void update();
        
            
            
            /*
            
            xxx update (xxx)
                Debounce krävs
                10 milisekunders period eller snabbare (Gäller alla komponenter)*/
    };
} 