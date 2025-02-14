#pragma once
#include <stdio.h>
#include "esp_mac.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"




namespace myBinaryLed {

    class binaryLed {

   
        private:
            ledc_mode_t speedMode;
            ledc_timer_t timerNumber;
            uint32_t timerFrequenceHertz;
            ledc_clk_cfg_t clockConfig;
            ledc_timer_bit_t dutyResolution;
            bool deconfigure;

           


        public:
            binaryLed(ledc_timer_t timerNumber, uint32_t timerFrequenceHertz, ledc_timer_bit_t dutyResolution);
            void init();

    };




    class ledChannel {
        private:
            int gpioNumber;
            ledc_channel_t channel;
            uint32_t dutyRange;
            int hpoint;
            ledc_sleep_mode_t sleepMode;
            unsigned int outInvert;
            ledc_intr_type_t interruptType;
    
            
        public:
            ledChannel(int gpioNumber,
                ledc_channel_t channel,
                uint32_t dutyRange,
                int hpoint,
                ledc_sleep_mode_t sleepMode,
                unsigned int outInvert,
                ledc_intr_type_t interruptType);
    };


}