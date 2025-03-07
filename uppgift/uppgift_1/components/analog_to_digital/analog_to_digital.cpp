#include <stdio.h>
#include "analog_to_digital.h"




namespace adcOneMode {  

    adc::adc(adc_unit_t unitId, adc_oneshot_clk_src_t clkSrc, adc_ulp_mode_t ulpMode, adc_channel_t adcChannel, adc_bitwidth_t adcBithWidth,  adc_atten_t adcAttenDB) 
    : unitId(unitId), clkSrc(clkSrc), ulpMode(ulpMode), adcChannel(adcChannel), adcBithWidth(adcBithWidth), adcAttenDB(adcAttenDB) {
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
        

        channelConf.bitwidth = this->adcBithWidth;
        channelConf.atten = this->adcAttenDB;

        // ADC_CHANNEL_2
        ESP_ERROR_CHECK(adc_oneshot_config_channel(this->adcUnitHandle, this->adcChannel, &channelConf));
        PRINTF_COLOR(ANSI_MAGENTA, "<-------------------------------------------ADC-Config----------------------------------------------->" NEW_LINE)
        PRINTF_GROUP_SUCCESFUL("oneshot mode and channel configuration for ADC worked!" NEW_LINE);
        PRINTF_GROUP_SUCCESFUL("adc_unit: %d & adc_channel: %d, configuration succed!" NEW_LINE, MY_ADC_UNIT, this->adcChannel);
        PRINTF_GROUP_SUCCESFUL("GPIO number for ADC is: %d" NEW_LINE, MY_GPIO_ADC);
        PRINTF_COLOR(ANSI_MAGENTA, "<------------------------------------------------------------------------------------------->" NEW_LINE);


        adc_cali_curve_fitting_config_t adcCaliConf;

        adcCaliConf.unit_id = this->getUnitId();
        adcCaliConf.chan = this->adcChannel;
        adcCaliConf.bitwidth = this->adcBithWidth;
        adcCaliConf.atten = this->adcAttenDB;
        

        ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&adcCaliConf, &adcCaliHandle));
    }

    void adc::onThreshold() {
        if(threshold_cb != NULL) {
            threshold_cb();
        }
    }


    void adc::update() {
        adc_oneshot_read(this->adcUnitHandle, this->adcChannel, &this->adc_raw_array[0][0]);
        
        int indexOfData = indexCounterFilter % 5;
        this->adcAveargeArray[indexOfData] = this->adc_raw_array[0][0];

        if(indexCounterFilter >= 4) {
            int sum = 0;
            for (int i = 0; i < 5; i++)
            {
                sum += this->adcAveargeArray[i];
            }

            int averageResult = sum / 5;

            

            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adcCaliHandle, averageResult, &voltage[0][0]));
            PRINTF_COLOR(ANSI_MAGENTA, "voltage in mV: %d" NEW_LINE, voltage[0][0]);
        
            
    
    
        }


       

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
      
        //PRINTF_COLOR(ANSI_MAGENTA, "ADC raw data: %d" NEW_LINE, this->adc_raw_array[0][0]);
    }

    void adc::setOnThreshold(int threshold, bool risingEdge, onThreshold_t onThreshHoldFunc) {
        this->threshold = threshold;
        this->risingEdge = risingEdge;

        this->threshold_cb = onThreshHoldFunc;
    }

    int adc::getValue() {
        return this->adc_raw_array[0][0];
    }

    int adc::getVoltageValueFromLDR() {
        adc_oneshot_read(this->adcUnitHandle, this->adcChannel, &this->adc_raw_array[0][0]);
        
        int indexOfData = indexCounterFilter % 5;
        this->adcAveargeArray[indexOfData] = this->adc_raw_array[0][0];

        if(indexCounterFilter >= 4) {
            int sum = 0;
            for (int i = 0; i < 5; i++)
            {
                sum += this->adcAveargeArray[i];
            }

            int averageResult = sum / 5;

            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adcCaliHandle, averageResult, &voltage[0][0]));
            //PRINTF_COLOR(ANSI_MAGENTA, "voltage in mV: %d" NEW_LINE, voltage[0][0]);
        }
        indexCounterFilter++;
        return voltage[0][0];
    }
}