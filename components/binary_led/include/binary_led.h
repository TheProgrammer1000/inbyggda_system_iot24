#pragma once
#include <stdio.h>
#include "esp_mac.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"


namespace myBinaryLed
{

    class binaryLed
    {
    private:
    
    int pin;
    gpio_mode_t mode;
    gpio_pullup_t pull_up_en;
    gpio_pulldown_t pull_down_en;
    gpio_int_type_t intr_type;

    int milisecLedOn;
    int milisecLedOff;

    TickType_t lastWakeTime;
    bool ledState; // false = off, true = on
    void ledsSetted();

    public:
        binaryLed(int pin, gpio_pullup_t pull_up_en, gpio_pulldown_t pull_down_en, gpio_int_type_t intr_type);
        void init();
        void blink(int milisecOn, int milisecOff);

        void update();

        typedef void (*setLed_t)(void);
        setLed_t setLed_cb;

        void setLed(setLed_t setLedFunc);
    };
}