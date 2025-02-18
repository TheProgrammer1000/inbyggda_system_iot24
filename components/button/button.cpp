#include <stdio.h>
#include "button.h"

namespace my_button {
    button::button(int pin, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type) : pin(pin), pull_up_en(pull_up_en), pull_down_en(pull_down_en), intr_type(intr_type)
    {
        gpio_mode_t mode = GPIO_MODE_INPUT;

        this->onPressed_cb = NULL;
        this->onRealsed_cb = NULL;
    };

    void button::init(){
        gpio_config_t button_config;

        button_config.pin_bit_mask = (uint64_t)pin;
        button_config.mode = mode;
        button_config.pull_up_en = pull_up_en;
        button_config.pull_down_en = pull_down_en;
        button_config.intr_type = intr_type;

        this->buttonRealsed = false;
        this->buttonPressed = false;
        
        ESP_ERROR_CHECK(gpio_config(&button_config));

        printf("Succesfully configured button\n");
    }

    void button::update() {
        gpio_num_t gpio_pin_number = (gpio_num_t)this->get_pin();
        int gpio_level = gpio_get_level(gpio_pin_number);

        if(gpio_level == 1 && buttonRealsed == false) {
            
            this->buttonRealsed = true;
            this->startTickButton = xTaskGetTickCount();

            if(this->onPressed_cb != NULL) {
                this->onPressed_cb(this->get_pin());
            }
        } 

        if(gpio_level == 0 && buttonRealsed == true) {
            TickType_t timeSincePressed = xTaskGetTickCount() - startTickButton;

            if(timeSincePressed >= pdMS_TO_TICKS(DEBOUNCE_DELAY)) {
                  
                if(this->onRealsed_cb != NULL) {
                    this->onRealsed_cb(this->get_pin());
                } 
                

                this->buttonRealsed = false;
                this->startTickButton = xTaskGetTickCount();
            }
        }
    }


    // får inte läsa pinnen, måste ta ett redan utläst värde
    bool button::isPressed() {
        return this->buttonRealsed;
    }

    void button::onPressed(int pin) {
        if(onPressed_cb != NULL) {
            onPressed_cb(pin);
        }
    }

    void button::onRealsed(int pin) {
        if(onRealsed_cb != NULL) {
            onRealsed_cb(pin);
        }
    }

    // parameter
    void button::setOnPressed(void(*cb)(int pin)) {
        onPressed_cb = cb;
    }    
    
    void button::setOnRealesed(void(*cb)(int pin)) {
        onRealsed_cb = cb;
    }   
}

