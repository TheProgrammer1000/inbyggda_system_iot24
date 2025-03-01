#include <stdio.h>
#include "analog_to_digital.h"




namespace adcOneMode {  

    adc::adc(adc_unit_t unitId, adc_oneshot_clk_src_t clkSrc, adc_ulp_mode_t ulpMode) : unitId(unitId), clkSrc(clkSrc), ulpMode(ulpMode) {
        threshold_cb = NULL;

        this->threshold = 0;
        this->risingEdge = true;

        this->thresholdState = false;

        indexCounterFilter = 0;
        
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


            threshold när den kommer över en visst threshold som user sätter då triggas denna funktion och som ska 
            printa ut värdet bara. Sen när den kommer under ett visst värde igen och kommer upp då ska den triggas.
    */


    void adc::onThreshold() {
        if(threshold_cb != NULL) {
            threshold_cb();
        }
    }

    void adc::update() {
        adc_oneshot_read(this->adcUnitHandle, MY_ADC_CHANNEL, &this->adc_raw_array[0][0]);


        /*
        
            int adcAveargeArray[10];
            int indexCounterFilter;
        */
        
        // int indexOfData = indexCounterFilter % 5;
        // this->adcAveargeArray[indexCounterFilter] = this->adc_raw_array[0][0];

        // if(indexCounterFilter >= 4) {
        //     int sum = 0;
        //     for (int i = 0; i < 5; i++)
        //     {
        //         sum += this->adcAveargeArray[i];
        //     }

        //     int averageResult = sum / 5;


                    
        //     //PRINTF_COLOR(ANSI_MAGENTA, "ADC channel: %d and averageResult: %d" NEW_LINE, MY_ADC_CHANNEL, averageResult);
        // }


        if(this->risingEdge == true) {
            if(this->threshold_cb != NULL && this->adc_raw_array[0][0] >= this->threshold && this->thresholdState == false) {
                threshold_cb();
                thresholdState = true;
            }
    
            if(this->adc_raw_array[0][0] < this->threshold) {
                thresholdState = false;
            }    
        }
        else {
            if(this->threshold_cb != NULL && this->adc_raw_array[0][0] <= this->threshold && this->thresholdState == false) {
                threshold_cb();
                thresholdState = true;
            }

            if(this->adc_raw_array[0][0] > this->threshold) {
                thresholdState = false;
            }   
        }

        

        indexCounterFilter++;
      
        PRINTF_COLOR(ANSI_MAGENTA, "ADC channel: %d and raw data: %d" NEW_LINE, MY_ADC_CHANNEL, this->adc_raw_array[0][0]);
    }

    void adc::setOnThreshold(int threshold, bool risingEdge, onThreshold_t onThreshHoldFunc) {
        this->threshold = threshold;
        this->risingEdge = risingEdge;

        this->threshold_cb = onThreshHoldFunc;
    }

    int adc::getValue() {
        return this->adc_raw_array[0][0];
    }
}