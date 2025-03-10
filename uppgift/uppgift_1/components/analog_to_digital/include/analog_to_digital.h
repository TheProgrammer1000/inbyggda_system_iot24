#pragma once
#include "hal/adc_types.h"
#include <esp_adc/adc_oneshot.h>
#include "esp_log.h"
#include "printer.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <cstdlib> 
#include <cmath>  



using namespace std;

#define WINDOW_FILTER 20
#define HYSTERESIS_THRESHOLD 7

#define AVERAGE_ARRAY_SIZE 5
#define MY_ADC_UNIT ADC_UNIT_1
#define MY_DIGI_CLK_SOURCE ADC_DIGI_CLK_SRC_DEFAULT


#define MY_ADC_BITWITH_BITS ADC_BITWIDTH_12
#define MY_ADC_ATTEN ADC_ATTEN_DB_12

#define MY_GPIO_ADC 2



static const char* TAG = "adcOneMode";


namespace adcOneMode {
    class adc {
        private:
            adc_oneshot_unit_handle_t adcUnitHandle;
            

            adc_unit_t unitId;             ///< ADC unit
            adc_oneshot_clk_src_t clkSrc;  ///< Clock source
            adc_ulp_mode_t ulpMode;        ///< ADC controlled by ULP, see `adc_ulp_mode_t`

            adc_atten_t atten;
            adc_bitwidth_t bitwidth;

            adc_channel_t adcChannel;
            adc_cali_handle_t adcCaliHandle;

            adc_bitwidth_t adcBithWidth;
            adc_atten_t adcAttenDB;
            


        public:
            adc(adc_unit_t unitId, adc_oneshot_clk_src_t clkSrc, adc_ulp_mode_t ulpMode, adc_channel_t adcChannel, adc_bitwidth_t adcBithWidth,  adc_atten_t adcAttenDB);
            int adc_raw_array[2][10];

            int lastStableVolt;
            
            
            int adcAveargeArray[WINDOW_FILTER];
            int indexCounterFilter;

            int threshold;
            bool risingEdge;

            bool thresholdState;

            typedef void (*onThreshold_t)(void);
            onThreshold_t threshold_cb;


            void setUnitId(adc_unit_t value) { unitId = value; }
            void setClkSrc(adc_oneshot_clk_src_t value) { clkSrc = value; }
            void setUlpMode(adc_ulp_mode_t value) { ulpMode = value; }

            void setAttenuation(adc_atten_t newAtten) {atten = newAtten; }
            void setBitwidth(adc_bitwidth_t newBitwidth) { bitwidth = newBitwidth; }  
            void setAdcUnitHandle(adc_oneshot_unit_handle_t handle) { adcUnitHandle = handle; }

            
            adc_oneshot_unit_handle_t getAdcUnitHandle() const { return adcUnitHandle; }
            adc_unit_t getUnitId() { return unitId; }
            adc_oneshot_clk_src_t getClkSrc() { return clkSrc; }
            adc_ulp_mode_t getUlpMode() { return ulpMode; }

            adc_atten_t getAtten() const {return atten; }
            adc_bitwidth_t getBitwidth() const { return bitwidth; }

            void calculateAdcAverage(int* adcAveargeArray, int sizeOfArray);
        
            /** @brief This extends to GPIO 2
            *   @attention Bit is 12 and atten: 12
            */    
            void init();
            void update();
            int getValue();

            int getVoltageValueFromLDR();

            void onThreshold();
            void setOnThreshold(int threshold, bool risingEdge, onThreshold_t onThreshHoldFunc);

    };
}
