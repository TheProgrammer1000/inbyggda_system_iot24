#pragma once
#include "printer.h"
#include "binary_led.h"
#include "adc_one.h"
#include <stdio.h>

namespace myAnalogLed {
    class analogLed {
        private:
            adcOneMode::adc adcInstance;
            myBinaryLed::binaryLed binaryLedInstance;

        public:
            analogLed(adcOneMode::adc& adcInstance, myBinaryLed::binaryLed& binaryLedInstance) : adcInstance(adcInstance), binaryLedInstance(binaryLedInstance) {}
            int adc_raw_array[2][10];

            void init();
            void update();
    };
}
