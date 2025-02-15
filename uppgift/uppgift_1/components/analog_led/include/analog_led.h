#pragma once
#include "printer.h"
#include "binary_led.h"
#include "adc_one.h"
#include <stdio.h>

namespace myAnalogLed {
    class analogLed {
        private:
            adcOneMode::adc* adcInstance;
            myBinaryLed::binaryLed* binaryLedInstance;

        public:
            analogLed() : adcInstance(nullptr), binaryLedInstance(nullptr) {}//adcInstance(MY_ADC_UNIT, MY_DIGI_CLK_SOURCE, ADC_ULP_MODE_DISABLE), binaryLedInstance(3, LEDC_CHANNEL_0, LEDC_TIMER_12_BIT, 0, LEDC_INTR_DISABLE) {

            typedef adcOneMode::adc* adc_t;
            typedef myBinaryLed::binaryLed* binaryLed_t;

            // analogLed::~analogLed() {
            //     if (adcInstance) {
            //         delete adcInstance;
            //         adcInstance = nullptr;
            //     }
            //     if(binaryLedInstance) {
            //         delete binaryLedInstance;
            //         binaryLedInstance = nullptr;
            //     }
            // }
            void config();
            void init();
    };
}
