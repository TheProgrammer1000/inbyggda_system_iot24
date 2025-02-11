#include <stdio.h>
#include "button.h"

namespace my_button {
    button::button(uint64_t pin_bit_mask, gpio_mode_t mode, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type) : pin_bit_mask(pin_bit_mask), mode(mode), pull_up_en(pull_up_en), pull_down_en(pull_down_en), intr_type(intr_type)
    {

    };

    void button::init(uint64_t pin, gpio_mode_t mode, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type){
        gpio_config_t button_config;

        button_config.pin_bit_mask = pin;
        button_config.mode = mode;
        button_config.pull_up_en = pull_up_en;
        button_config.pull_down_en = pull_down_en;
        button_config.intr_type = GPIO_INTR_POSEDGE;

        this->buttonPressed = false;
        
        ESP_ERROR_CHECK(gpio_config(&button_config));

        printf("Succesfully configured button\n");
    }

    void button::update() {
        gpio_num_t gpio_pin_number = (gpio_num_t)this->pin_bit_mask;
        int gpio_level = gpio_get_level(gpio_pin_number);

        if(gpio_level == 1 && buttonPressed == false) {
            
            this->buttonPressed = true;
            this->startTickButton = xTaskGetTickCount();
        }
        if(buttonPressed == true) {
            TickType_t timeSincePressed = xTaskGetTickCount() - startTickButton;

            if(timeSincePressed >= pdMS_TO_TICKS(10)) {
                

                printf("Button Pressed!\n");
                this->buttonPressed = false;
                this->startTickButton = xTaskGetTickCount();
            }
        }
    }


    // får inte läsa pinnen, måste ta ett redan utläst värde
    bool button::isPressed() {
        return this->buttonPressed;
    }

    void button::onPressed(int pin) {

    }

    void button::setOnPressed(void(*onPressed)(int pin)) {
      
    }    
   
}

