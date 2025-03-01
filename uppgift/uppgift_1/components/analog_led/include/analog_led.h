#pragma once
#include "printer.h"
#include "binary_led.h"
#include <stdio.h>
#include <cmath>         // for sin() and M_PI
#define BUFFER_SIZE 5


#define LEDC_TIMER_NUMBER LEDC_TIMER_0
#define SPEED_MODE LEDC_LOW_SPEED_MODE
#define CLOCK_CONFIG LEDC_AUTO_CLK
#define TIMER_FREQ_HERTZ 4000



namespace myAnalogLed {
    class analogLed {
        private:
        // @brief Timer
        ledc_mode_t speedMode;
        ledc_timer_t timerNumber;
        uint32_t timerFrequenceHertz;
        ledc_timer_bit_t dutyResolution;
        bool deconfigure;

        // @brief Channel
        int gpioNumber;
        ledc_channel_t channel;
        uint32_t dutyPeekForSineWave;
        int hpoint;
        ledc_sleep_mode_t sleepMode;
        //unsigned int outInvert;
        ledc_intr_type_t interruptType;

        double sinePeriod;

        bool fadeOut;
        
        int setLedValue;
        bool isLedSet;


        public:
        /**
        * @note timer must be configure before! and give the timerNumber only!
        */
       analogLed(int gpioNumber, ledc_timer_t timerNumber, ledc_channel_t channel, ledc_timer_bit_t dutyResolution, uint32_t dutyPeekForSineWave, ledc_intr_type_t interruptType);
   
        // Getters  Timer
        ledc_mode_t getSpeedMode() const { return speedMode; }
        ledc_timer_t getTimerNumber() const { return timerNumber; }
        uint32_t getTimerFrequenceHertz() const { return timerFrequenceHertz; }
        ledc_timer_bit_t getDutyResolution() const { return dutyResolution; }
        bool getDeconfigure() const { return deconfigure; }

        // Setters  Timer
        void setSpeedMode(ledc_mode_t mode) { speedMode = mode; }
        void setTimerNumber(ledc_timer_t number) { timerNumber = number; }
        void setTimerFrequenceHertz(uint32_t frequency) { timerFrequenceHertz = frequency; }
        void setDutyResolution(ledc_timer_bit_t resolution) { dutyResolution = resolution; }
        void setDeconfigure(bool value) { deconfigure = value; }

        int getGpioNumber() const { return gpioNumber; }
        void setGpioNumber(int gpio) { gpioNumber = gpio; }
    
        ledc_channel_t getChannel() const { return channel; }
        void setChannel(ledc_channel_t ch) { channel = ch; }
    
        uint32_t getDutyRange() const { return dutyPeekForSineWave; }
        void setDutyRange(uint32_t duty) { dutyPeekForSineWave = duty; }

        ledc_intr_type_t getInterruptType() const { return interruptType; }
        void setInterruptType(ledc_intr_type_t intrType) { interruptType = intrType; }

        int getHpoint() const { return hpoint; }
        void setHpoint(int value) { hpoint = value; }
        
        ledc_sleep_mode_t getSleepMode() const { return sleepMode; }
        void setSleepMode(ledc_sleep_mode_t mode) { sleepMode = mode; }


        uint32_t sineWave(double period); // Helper function to compute duty from sine wave

        void setLed(int setLedValue);
        /**
        * 
        * @attention Initializes only channel configuration.     
        *
        */
        void init();
        void update();
    };
}
