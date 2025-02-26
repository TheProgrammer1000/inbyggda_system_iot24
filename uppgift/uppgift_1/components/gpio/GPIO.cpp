#include "GPIO.h"

namespace myGpio {
    Gpio::Gpio(int pin, gpio_mode_t mode, gpio_pullup_t pullUpEnable, gpio_pulldown_t pullDownEnable, gpio_int_type_t interuptType) {
        this->pin = pin;
        this->mode = mode;
        this->pullUpEnable = pullUpEnable;
        this->pullDownEnable = pullDownEnable;
        this->interuptType = interuptType;
    }   

    
    void Gpio::init() {
        gpio_config_t gpioConf;

        gpioConf.intr_type = this->interuptType;
        gpioConf.mode = this->mode;
        gpioConf.pin_bit_mask = (1UL << this->pin);
        gpioConf.pull_down_en = this->pullDownEnable;
        gpioConf.pull_up_en = this->pullUpEnable;
        
        ESP_ERROR_CHECK(gpio_config(&gpioConf));
        gpio_dump_io_configuration(stdout, (1UL << this->pin));
    }
    void Gpio::update() {
        int level = gpio_get_level((gpio_num_t)this->pin);

        printf("level: %d\n", level);
        // if(level > 0) {
        //     PRINTF_COLOR(ANSI_BLUE, "THE INPUT GOT SOMETHING!" NEW_LINE);
        // }
        // else {
        //     PRINT_COLOR(ANSI_BLUE, "THE INPUT GOT SOMETHING!" NEW_LINE);
        // }
        

    }
}
