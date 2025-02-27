#pragma once
#include "GPIO.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "driver/dedic_gpio.h"

namespace myBitBang {
    class bitBang {
        private:
            myGpio::Gpio gpioTransmitter;
            myGpio::Gpio gpioReciever;
            int baudRate;
            int level;
            bool isHighOrLow;
            TickType_t tickSinceStart;


        public:
            bitBang(myGpio::Gpio& gpioTransmitter, myGpio::Gpio& gpioReciever) : gpioTransmitter(gpioTransmitter), gpioReciever(gpioReciever) {}
            void init(int setBaudRate);

            const myGpio::Gpio& getGpioTransmitter() const {
                return gpioTransmitter;
            }

            const myGpio::Gpio& getGpioReciever() const {
                return gpioReciever;
            }
    
            void setGpioTransmitter(const myGpio::Gpio& newGpioTransmitter) {
                gpioTransmitter = newGpioTransmitter;
            }
        
            void setGpioReciever(const myGpio::Gpio& newGpioReciever) {
                gpioReciever = newGpioReciever;
            }

            void update();
    };
}
