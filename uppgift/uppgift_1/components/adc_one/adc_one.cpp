#include <stdio.h>
#include "adc_one.h"




namespace adcOneMode { 

    adc::adc(adc_unit_t unitId, adc_oneshot_clk_src_t clkSrc, adc_ulp_mode_t ulpMode) : unitId(unitId), clkSrc(clkSrc), ulpMode(ulpMode) {
        adcAveargeData = (int*) calloc(AVERAGE_ARRAY_SIZE, sizeof(int));

        onThreshold_cb = NULL;

        if(adcAveargeData == NULL) {
            printf("Allocation failed!\n");
            exit(0);
        }

        for (int i = 0; i < AVERAGE_ARRAY_SIZE; i++)
        {
            adcAveargeData[i] = -1;
        }
        
    }





    void adc::init() {
        //adc_oneshot_unit_init_cfg_t adcConf;
        //adc_oneshot_unit_handle_t adcUnitHandle;
        adc_oneshot_unit_init_cfg_t adcConf;

        adcConf.unit_id = this->getUnitId();
        adcConf.clk_src = this->getClkSrc();
        adcConf.ulp_mode = this->getUlpMode();

        ESP_ERROR_CHECK(adc_oneshot_new_unit(&adcConf, &this->adcUnitHandle));

        adc_oneshot_chan_cfg_t channelConf;

        channelConf.bitwidth = ADC_BITWIDTH_DEFAULT;
        channelConf.atten = ADC_ATTEN_DB_12;

  
        // ADC_CHANNEL_2

        ESP_ERROR_CHECK(adc_oneshot_config_channel(this->adcUnitHandle, MY_ADC_CHANNEL, &channelConf));
        PRINTF_COLOR(ANSI_BLUE, "adc_unit: %d & adc_channel: %d, configuration succed!" NEW_LINE, MY_ADC_UNIT, MY_ADC_CHANNEL);

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