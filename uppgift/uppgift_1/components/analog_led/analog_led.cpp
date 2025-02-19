#include "analog_led.h"

namespace myAnalogLed {

    analogLed::analogLed(int gpioNumber, ledc_channel_t channel,  ledc_timer_bit_t dutyResolution, uint32_t dutyRange, ledc_intr_type_t interruptType){
        
        this->timerNumber = LEDC_TIMER_NUMBER;
        this->timerFrequenceHertz = TIMER_FREQ_HERTZ;
        this->dutyResolution = dutyResolution;
        this->speedMode = LEDC_LOW_SPEED_MODE;
        this->clockConfig = CLOCK_CONFIG;
        this->deconfigure = false;

        this->interruptType = interruptType;
        this->dutyRange = dutyRange;
        this->gpioNumber = gpioNumber;    
        this->channel = channel;
        
        this->hpoint = 0;
        this->sleepMode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;

        this->isLedSet = false;

    }

    void analogLed::init() {
        ledc_timer_config_t ledc_timer;
        
        ledc_timer.speed_mode = this->speedMode;
        ledc_timer.timer_num = this->timerNumber;
        ledc_timer.freq_hz = this->timerFrequenceHertz;
        ledc_timer.clk_cfg = this->clockConfig;
        ledc_timer.duty_resolution = this->dutyResolution; 
        ledc_timer.deconfigure = this->deconfigure;

        esp_err_t timer_err_esp = ledc_timer_config(&ledc_timer);

        if(timer_err_esp == ESP_OK) {
            PRINTF_GROUP_SUCCESFUL("Successfully configured ledc timer configuration!" NEW_LINE);

            ledc_channel_config_t ledc_channel_conf;
            ledc_channel_conf.gpio_num =            this->gpioNumber;
            ledc_channel_conf.speed_mode =          this->speedMode;
            ledc_channel_conf.channel =             this->channel;
            ledc_channel_conf.duty    =             0;  // default setting this to 0 and already assign user value
            ledc_channel_conf.hpoint  =             0;
            ledc_channel_conf.intr_type =           this->interruptType;
            ledc_channel_conf.timer_sel =           ledc_timer.timer_num;
            ledc_channel_conf.sleep_mode =          LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
            

            esp_err_t esp_err = ledc_channel_config(&ledc_channel_conf);

            if(esp_err == ESP_ERR_INVALID_ARG) {
                PRINTF_COLOR_WITH_LINE(ANSI_RED,"Invalid parameters");
            }
            else if(esp_err == ESP_OK) {
                PRINTF_GROUP_SUCCESFUL("Successfully configured channel!" NEW_LINE);
            }

        }
        else if(timer_err_esp == ESP_ERR_INVALID_ARG) {
            PRINTF_GROUP_FAILED("Parameter error!\n");
        }
        else if(timer_err_esp == ESP_FAIL) {
            PRINTF_GROUP_FAILED("Can not find a proper pre-divider number base on the given frequency and the current duty_resolution\n");
        }
        else if(timer_err_esp == ESP_ERR_INVALID_STATE) {
            PRINTF_GROUP_FAILED("Timer cannot be de-configured because timer is not configured or is not paused");
        }
    }

     // Helper function that computes a PWM duty cycle based on a sine wave.
    // The 'periodInTicks' parameter is the duration of one full sine cycle in ticks.
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
    
        // Map sineValue from [-1, 1] to [0, dutyRange]
        uint32_t duty = static_cast<uint32_t>(std::round(((sineValue + 1.0) / 2.0) * dutyRange));
        return duty;
    }
    



    void analogLed::setLed(int setLedValue) {
        this->isLedSet = true;
        this->setLedValue = setLedValue;
    }

    void analogLed::update() {
        
        if(this->isLedSet == false) {

            this->setLedValue = sineWave(this->sinePeriod);

            // Update the PWM duty cycle accordingly.
            ledc_set_duty(speedMode, channel, this->setLedValue);
            ledc_update_duty(speedMode, channel);
            //TickType_t periodInTick = pdMS_TO_TICKS(2000); 

            // // Get the duty cycle based on the sine wave
            // //uint32_t duty = sineWave();
    
            // ledc_set_duty(speedMode, channel, duty);
            // ledc_update_duty(speedMode, channel);
        }
        else {
            ledc_set_duty(speedMode, channel, this->setLedValue);
            ledc_update_duty(speedMode, channel);
        }
    }
}
