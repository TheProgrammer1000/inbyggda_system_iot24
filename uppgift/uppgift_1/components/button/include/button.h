/*
    xxx init( pin, xxx )
        Ska gå att ha flera knappar samtidigt, gäller för alla komponenter
        För cpp får konstruktorn anvöndas för parametrar, men inte för att faktiskt “sätta upp/confugurera esp-idf”

*/

#pragma once
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DEBOUNCE_DELAY 30

namespace my_button {
    class button {
        private:
         

        public:
            button(int pin, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type);
            
            int pin;          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
            gpio_mode_t mode;               /*!< GPIO mode: set input/output mode                     */
            gpio_pullup_t pull_up_en;       /*!< GPIO pull-up                                         */
            gpio_pulldown_t pull_down_en;   /*!< GPIO pull-down                                       */
            gpio_int_type_t intr_type;      /*!< GPIO interrupt type         */ 

            typedef void (*onPressed_t)(int pin);

            onPressed_t onPressed_cb;
            onPressed_t onRealsed_cb;


            bool buttonRealsed;
            bool buttonPressed;
            TickType_t startTickButton;

            void init();
            bool isPressed();
            void onPressed(int pin);
            void onRealsed(int pin);

            void setOnPressed(onPressed_t onPressed);
            void setOnRealesed(onPressed_t onRealsed);
            void setupInterrupt();
            
            void update();
        
            
            
            /*
            
            xxx update (xxx)
                Debounce krävs
                10 milisekunders period eller snabbare (Gäller alla komponenter)*/
    };
} 