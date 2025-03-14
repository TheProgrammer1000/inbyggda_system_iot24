#include "GPIO.h"



namespace myGpio {
    Gpio::Gpio(int pin, gpio_mode_t mode, gpio_pullup_t pullUpEnable, gpio_pulldown_t pullDownEnable, gpio_int_type_t interuptType) {
        this->pin = pin;
        this->mode = mode;
        this->pullUpEnable = pullUpEnable;
        this->pullDownEnable = pullDownEnable;
        this->interuptType = interuptType;

        this->isrHandler = nullptr;
        this->IsrArg = nullptr;
        this->glitchFilterHandle = nullptr;

        this->isrServiceInstalled = false;
        this->gpioInterruptTriggered = false;
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

    void Gpio::attachInterruptToPin(void (*isrCallBackFunc)(void *data), void* arg) {
        // Install the ISR service if it hasn't been installed already.
        if (!isrServiceInstalled) {
            ESP_ERROR_CHECK(gpio_install_isr_service(0));
            isrServiceInstalled = true;
        }


        gpio_pin_glitch_filter_config_t glitchFilterConf;

        glitchFilterConf.gpio_num = (gpio_num_t)this->pin;
        glitchFilterConf.clk_src = GLITCH_FILTER_CLK_SRC_DEFAULT;

        ESP_ERROR_CHECK(gpio_new_pin_glitch_filter(&glitchFilterConf, &glitchFilterHandle));
        

        ESP_ERROR_CHECK(gpio_isr_handler_add((gpio_num_t)this->pin, isrCallBackFunc, arg));
    }
    

    void Gpio::detachInterruptToPin() {
        if(isrServiceInstalled == true) {
            ESP_ERROR_CHECK(gpio_isr_handler_remove((gpio_num_t)this->pin));
            ESP_ERROR_CHECK(gpio_del_glitch_filter(glitchFilterHandle));
            gpio_uninstall_isr_service();
        }
        else {
            PRINTF_COLOR(ANSI_YELLOW, "No isr handler is attached to this pin: %d" NEW_LINE, this->pin);
        }
    }
}
