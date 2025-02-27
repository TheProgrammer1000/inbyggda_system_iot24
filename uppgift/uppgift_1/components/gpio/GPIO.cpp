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


    void IRAM_ATTR Gpio::isrCallBackFunc(void* data) {
        // ISR implementation
        // Cast the data pointer back to your Gpio instance
        Gpio* instance = static_cast<Gpio*>(data);
        // Now you can modify member variables, e.g.,
        instance->gpioInterruptTriggered = true;

        //esp_rom_printf("Hello from ISR! gpioInterruptTriggered set to: %d\n",  instance->gpioInterruptTriggered);
    
    }

    void Gpio::attachInterrupt() {
        // Install the ISR service if it hasn't been installed already.
        if (!isrServiceInstalled) {
            ESP_ERROR_CHECK(gpio_install_isr_service(0));
            isrServiceInstalled = true;
        }
        
        ESP_ERROR_CHECK(gpio_isr_handler_add((gpio_num_t)this->pin, Gpio::isrCallBackFunc, this));
    }
}
