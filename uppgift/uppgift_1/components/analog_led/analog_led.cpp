#include "analog_led.h"

namespace myAnalogLed {

    analogLed::analogLed(int gpioNumber, ledc_timer_t timerNumber, ledc_channel_t channel, ledc_timer_bit_t dutyResolution, uint32_t dutyPeekForSineWave, ledc_intr_type_t interruptType){
        
        this->gpioNumber = gpioNumber;  
        this->timerFrequenceHertz = 4000;
        this->dutyResolution = dutyResolution;
        this->speedMode = LEDC_LOW_SPEED_MODE;
        this->channel = channel;
        
        this->timerNumber = timerNumber;
        this->dutyPeekForSineWave = dutyPeekForSineWave;
        this->interruptType = interruptType;

        this->isLedSet = false;
    }

    void analogLed::init() {

        ledc_channel_config_t ledc_channel_conf;
        ledc_channel_conf.gpio_num =            this->gpioNumber;
        ledc_channel_conf.speed_mode =          this->speedMode;
        ledc_channel_conf.channel =             this->channel;
        ledc_channel_conf.intr_type =           this->interruptType;
        ledc_channel_conf.timer_sel =           this->timerNumber;
        ledc_channel_conf.sleep_mode =          LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;

        ledc_channel_conf.duty    =             0;  // default setting this to 0 and already assign user value
        ledc_channel_conf.hpoint  =             0;



        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_conf));
        PRINTF_COLOR(ANSI_MAGENTA, "Successfully configured analogLed on pin: %d!" NEW_LINE, this->gpioNumber);

    }

    /**
     * @brief 
     * @attention call this with pdMS_TO_TICKS to milisec in the argument like 'pdMS_TO_TICKS(2000)'!
     * @param periodInTicks 
     * 
     */
    uint32_t analogLed::sineWave(double periodInTicks) {
        // Store the start tick count (only once)
        if(isLedSet == true) {
            isLedSet = false;
        }

        this->sinePeriod = periodInTicks;

        static TickType_t startTick = xTaskGetTickCount();
    
        // Get the current tick count
        TickType_t currentTick = xTaskGetTickCount();
        TickType_t elapsedTicks = currentTick - startTick;
    
        // Calculate the phase as a fraction of the period [0, 1)
        double phase = fmod(static_cast<double>(elapsedTicks), periodInTicks) / periodInTicks;
    
        // Convert the phase to an angle (0 to 2π radians)
        double angle = phase * 2.0 * M_PI;
    
        // Compute the sine value (range: -1 to 1)
        double sineValue = sin(angle);
    
        // Map sineValue from [-1, 1] to [0, dutyPeekForSineWave]
        uint32_t duty = static_cast<uint32_t>(std::round(((sineValue + 1.0) / 2.0) * this->dutyPeekForSineWave));

        return duty;
    }
    



    void analogLed::setLed(int setLedValue) {
        this->isLedSet = true;
        this->setLedValue = setLedValue;
    }

    void analogLed::update() {
        
        if(this->isLedSet == false) {
            this->setLedValue = sineWave(this->sinePeriod);

            ledc_set_duty(speedMode, channel, this->setLedValue);
            ledc_update_duty(speedMode, channel);
        }
        else {
            ledc_set_duty(speedMode, channel, this->setLedValue);
            ledc_update_duty(speedMode, channel);
        }
    }
}
