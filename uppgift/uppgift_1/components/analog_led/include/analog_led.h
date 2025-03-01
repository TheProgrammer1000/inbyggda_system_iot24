#pragma once
#include "printer.h"
#include "binary_led.h"
#include <stdio.h>
#include <cmath>         // for sin() and M_PI
#define BUFFER_SIZE 5


#define LEDC_TIMER_NUMBER LEDC_TIMER_0
#define SPEED_MODE LEDC_LOW_SPEED_MODE
#define CLOCK_CONFIG LEDC_AUTO_CLK
#define TIMER_FREQ_HERTZ 1000



namespace myAnalogLed {
    class analogLed {
        private:
        // @brief Timer
        ledc_mode_t speedMode;
        ledc_timer_t timerNumber;
        uint32_t timerFrequenceHertz;
        ledc_clk_cfg_t clockConfig;
        ledc_timer_bit_t dutyResolution;
        bool deconfigure;

        // @brief Channel
        int gpioNumber;
        ledc_channel_t channel;
        uint32_t dutyRange;
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
        * @note configuration parameters are set to default in the constructor:
        *
        * - LEDC_TIMER =            @b LEDC_TIMER_0
        *
        * - TIMER_FREQ_HERTZ =      @b 1000
        *
        * - LEDC_CLK =              @b LEDC_AUTO_CLK
        *
        * - TIMER_DUTY_RESOLUTION = @b LEDC_TIMER_10_BIT
        *
        * - LEDC_DECONFIGURE =      @b FALSE
        * 
        * @attention if you want to change these call setter before init()
        */
        analogLed(int gpioNumber, ledc_channel_t channel, ledc_timer_bit_t dutyResolution, uint32_t dutyRange, ledc_intr_type_t interruptType);
   
        // Getters  Timer
        ledc_mode_t getSpeedMode() const { return speedMode; }
        ledc_timer_t getTimerNumber() const { return timerNumber; }
        uint32_t getTimerFrequenceHertz() const { return timerFrequenceHertz; }
        ledc_clk_cfg_t getClockConfig() const { return clockConfig; }
        ledc_timer_bit_t getDutyResolution() const { return dutyResolution; }
        bool getDeconfigure() const { return deconfigure; }

        // Setters  Timer
        void setSpeedMode(ledc_mode_t mode) { speedMode = mode; }
        void setTimerNumber(ledc_timer_t number) { timerNumber = number; }
        void setTimerFrequenceHertz(uint32_t frequency) { timerFrequenceHertz = frequency; }
        void setClockConfig(ledc_clk_cfg_t config) { clockConfig = config; }
        void setDutyResolution(ledc_timer_bit_t resolution) { dutyResolution = resolution; }
        void setDeconfigure(bool value) { deconfigure = value; }

        int getGpioNumber() const { return gpioNumber; }
        void setGpioNumber(int gpio) { gpioNumber = gpio; }
    
        ledc_channel_t getChannel() const { return channel; }
        void setChannel(ledc_channel_t ch) { channel = ch; }
    
        uint32_t getDutyRange() const { return dutyRange; }
        void setDutyRange(uint32_t duty) { dutyRange = duty; }

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
        * @brief Initializes the timer and channel configuration.
        *
        * @note Timer_conf variables are already initialized in the constructor.
        *
        * @attention This function initializes both the timer and channel.
        *          Use the timer_conf setter if you need to change the configuration.
        *
        */
        void init();
        void update();
    };
}
