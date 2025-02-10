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
        button_config.intr_type = intr_type;
        
        ESP_ERROR_CHECK(gpio_config(&button_config));

        printf("Succesfully configured button\n");
    }
}

