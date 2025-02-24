#include <stdio.h>
#include "binary_led.h"
namespace myBinaryLed {
    binaryLed::binaryLed(int pin, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type) {
        this->pin = pin;
        this->mode = GPIO_MODE_OUTPUT;
        this->pull_up_en = pull_up_en;
        this->pull_down_en = pull_down_en;
        this->intr_type = intr_type;

        isSetLed = false;
        setLedValue = -1;
    
    };

    void binaryLed::init() {
       gpio_config_t gpioConf;

       gpioConf.pin_bit_mask = (1UL << this->pin);
       gpioConf.mode = this->mode;
       gpioConf.pull_up_en = this->pull_up_en;
       gpioConf.pull_down_en = this->pull_down_en;
       gpioConf.intr_type = this->intr_type;

        ESP_ERROR_CHECK(gpio_config(&gpioConf));
        PRINTF_COLOR(ANSI_MAGENTA, "Sucessfully configured binary pin: %d" NEW_LINE, this->pin);

        // Initialize lastWakeTime after the scheduler is running
        lastWakeTime = xTaskGetTickCount();
        ledState = false; // Assuming LED starts off
    }

    void binaryLed::update() {

        TickType_t currentTick = xTaskGetTickCount();
        
        if(this->isSetLed == false) {
            if (ledState) {  // LED is currently ON
                if ((currentTick - lastWakeTime) >= pdMS_TO_TICKS(milisecLedOn)) {
                    gpio_set_level((gpio_num_t)this->pin, 0);
                    ledState = false;
                    lastWakeTime = currentTick;       
                }
            } else {  // LED is currently OFF
                if ((currentTick - lastWakeTime) >= pdMS_TO_TICKS(milisecLedOff)) {
                    gpio_set_level((gpio_num_t)this->pin, 1);
                    ledState = true;
                    lastWakeTime = currentTick;    
                }
            }
        } else {
            gpio_set_level((gpio_num_t) this->pin,this->setLedValue);
        }

        
    }

    void binaryLed::blink(int miliSecOn, int miliSecOff) {
        this->milisecLedOn = miliSecOn;
        this->milisecLedOff = miliSecOff;
    }


    void binaryLed::setLed(int setLedValue) {
        this->isSetLed = true;
        this->setLedValue = setLedValue;
        
    }
}
