#include <stdio.h>
#include "adc_one.h"




namespace adcOneMode {  

    adc::adc(adc_unit_t unitId, adc_oneshot_clk_src_t clkSrc, adc_ulp_mode_t ulpMode) : unitId(unitId), clkSrc(clkSrc), ulpMode(ulpMode) {
        onThreshold_cb = NULL;
        
    }

    void adc::init() {
        adc_oneshot_unit_init_cfg_t adcConf;

        adcConf.unit_id = this->getUnitId();
        adcConf.clk_src = this->getClkSrc();
        adcConf.ulp_mode = this->getUlpMode();

        ESP_ERROR_CHECK(adc_oneshot_new_unit(&adcConf, &this->adcUnitHandle));

        adc_oneshot_chan_cfg_t channelConf;

        channelConf.bitwidth = MY_ADC_BITWITH_BITS;
        channelConf.atten = MY_ADC_ATTEN;

        // ADC_CHANNEL_2
        ESP_ERROR_CHECK(adc_oneshot_config_channel(this->adcUnitHandle, MY_ADC_CHANNEL, &channelConf));
        PRINTF_COLOR(ANSI_MAGENTA, "<-------------------------------------------ADC-Config----------------------------------------------->" NEW_LINE)
        PRINTF_GROUP_SUCCESFUL("oneshot mode and channel configuration for ADC worked!" NEW_LINE);
        PRINTF_GROUP_SUCCESFUL("adc_unit: %d & adc_channel: %d, configuration succed!" NEW_LINE, MY_ADC_UNIT, MY_ADC_CHANNEL);
        PRINTF_GROUP_SUCCESFUL("GPIO number for ADC is: %d" NEW_LINE, MY_GPIO_ADC);
        PRINTF_COLOR(ANSI_MAGENTA, "<------------------------------------------------------------------------------------------->" NEW_LINE);
    }



    /*
            typedef void (*onThreshold_t)(int pin, int value);
            onThreshold_t onThreshold_cb;

            void setOnThreshold(int threshold, bool risingEdge, onThreshold_t onThreshHoldFunc);
    
    */

    void adc::update() {
        adc_oneshot_read(this->adcUnitHandle, MY_ADC_CHANNEL, &this->adc_raw_array[0][0]);

        if(this->onThreshold_cb != NULL) {

        }
        
        PRINTF_COLOR(ANSI_MAGENTA, "ADC channel: %d and raw data: %d" NEW_LINE, MY_ADC_CHANNEL, this->adc_raw_array[0][0]);
      
    }

    // int adc::getValue() {

    // }

    void adc::setOnThreshold(int threshold, bool risingEdge, onThreshold_t onThreshHoldFunc) {
        this->threshold = threshold;
        this->risingEdge = risingEdge;
        
        this->onThreshold_cb = onThreshHoldFunc;
    }
}