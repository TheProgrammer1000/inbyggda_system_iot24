#pragma once
#include <stdio.h>
#include "esp_log.h"
#include "binary_led.h"
#include "analog_led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

namespace myLedController {
    class ledController {
        private:
            myAnalogLed::analogLed* analogLeds;
            myBinaryLed::binaryLed* binaryLeds;

            int sizeOfBinary;
            int sizeOfAnalog;

            TickType_t lastWakeTimeOnSnake;
            TickType_t lastWakeTimeOnPeriod;

            bool ledState; // false = off, true = on

        public:
            ledController();
            void blinkAll(int onMs, int offMs);

            void myLedAnimation(int lapPeriodMs, int sizeOfBinary, bool isAnalog);

            void snakeAnimation(int length, int lapPeriodMs);
            
            myAnalogLed::analogLed* getAnalogLeds() {return analogLeds;};
            myBinaryLed::binaryLed* getBinaryLeds() {return binaryLeds;};

            void init(myBinaryLed::binaryLed binaryLeds[], int sizeOfBinary, myAnalogLed::analogLed analogLeds[], int sizeOfAnalog);
    };
}
