#pragma once
#include <stdio.h>
#include "binary_led.h"
#include "analog_led.h"

namespace myLedController {
    class ledController {
        private:
            myAnalogLed::analogLed* analogLeds;
            myBinaryLed::binaryLed* binaryLeds;

        public:
            ledController();
            void init(myBinaryLed::binaryLed binaryLeds[], int sizeOfBinary, myAnalogLed::analogLed analogLeds[], int sizeOfAnalog);
    };
}
