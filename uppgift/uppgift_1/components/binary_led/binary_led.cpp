#include <stdio.h>
#include "binary_led.h"
namespace myBinaryLed {
    binaryLed::binaryLed(int gpioNumber, ledc_channel_t channel,  ledc_timer_bit_t dutyResolution, uint32_t dutyRange, ledc_intr_type_t interruptType){
        
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
    };

    void binaryLed::init() {
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
            ledc_channel_conf.gpio_num =            3;
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

    void binaryLed::blink(int milisecOn, int milisecOff) {
        ledc_set_duty(this->speedMode, this->channel, this->dutyRange); 
        ledc_update_duty(this->speedMode, this->channel);
        vTaskDelay(pdMS_TO_TICKS(milisecOn));

        ledc_set_duty(this->speedMode, this->channel, 0); 
        ledc_update_duty(this->speedMode, this->channel);
        vTaskDelay(pdMS_TO_TICKS(milisecOff));
    }
};
