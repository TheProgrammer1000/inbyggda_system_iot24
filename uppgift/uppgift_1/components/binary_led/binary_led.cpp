#include <stdio.h>
#include "binary_led.h"
namespace myBinaryLed {

/*

            ledc_timer.timer_num = LEDC_TIMER_0;
        ledc_timer.freq_hz = 1000;
        ledc_timer.clk_cfg = LEDC_AUTO_CLK;
        ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
        ledc_timer.deconfigure = false;
        
        esp_err_t timer_err_esp = ledc_timer_config(&ledc_timer);

*/


    binaryLed::binaryLed(ledc_timer_t timerNumber, uint32_t timerFrequenceHertz, ledc_timer_bit_t dutyResolution) : timerNumber(timerNumber), timerFrequenceHertz(timerFrequenceHertz), dutyResolution(dutyResolution){
        this->speedMode = LEDC_LOW_SPEED_MODE;
        this->clockConfig = LEDC_AUTO_CLK;
        this->deconfigure = false;
    };

    void binaryLed::init() {
        ledc_timer_config_t ledc_timer;
        
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
            ledc_channel_conf.speed_mode =          LEDC_LOW_SPEED_MODE;
            ledc_channel_conf.channel =             LEDC_CHANNEL_0;
            ledc_channel_conf.duty    =             0b1111111111;
            ledc_channel_conf.hpoint  =             0;
            ledc_channel_conf.flags.output_invert = 0;
            ledc_channel_conf.intr_type =           LEDC_INTR_DISABLE;
            ledc_channel_conf.timer_sel =           ledc_timer.timer_num;
            ledc_channel_conf.sleep_mode =          LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
            

            esp_err_t esp_err = ledc_channel_config(&ledc_channel_conf);

            if(esp_err == ESP_ERR_INVALID_ARG) {
                PRINTF_COLOR_WITH_LINE(ANSI_RED,"Invalid parameters");
            }
            else if(esp_err == ESP_OK) {
                PRINTF_GROUP_SUCCESFUL("Successfully configured channel!" NEW_LINE);
                

                vTaskDelay(pdMS_TO_TICKS(5000));

                ledc_set_duty(LEDC_LOW_SPEED_MODE,  ledc_channel_conf.channel, 0b1111111111 / 4);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, ledc_channel_conf.channel);

                PRINTF_GROUP_SUCCESFUL("Blinker lightning lowered!!" NEW_LINE);
                
                vTaskDelay(pdMS_TO_TICKS(5000));
                while(1) {
                    vTaskDelay(pdMS_TO_TICKS(30));
                }
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



    // ledChannel::ledChannel(int gpioNumber,
    //     ledc_channel_t channel,
    //     uint32_t dutyRange,
    //     int hpoint,
    //     ledc_sleep_mode_t sleepMode,
    //     unsigned int outInvert,
    //     ledc_intr_type_t interruptType);
};
