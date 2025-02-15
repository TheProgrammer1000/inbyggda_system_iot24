#include "analog_led.h"

namespace myAnalogLed {

    void analogLed::config() {
        analogLed::adcInstance = (adc_t) malloc(sizeof(adc_t));
        analogLed::binaryLedInstance = (binaryLed_t) malloc(sizeof(binaryLed_t));

        
    }
    
    void analogLed::init() {
       // this->adcInstance.init();
        //this->binaryLedInstance.init();
    }


    // analogLed::analogLed() {
    //     (this->instance)(MY_ADC_UNIT, MY_DIGI_CLK_SOURCE, ADC_ULP_MODE_DISABLE);
    //     //this->myAdc(MY_ADC_UNIT, MY_DIGI_CLK_SOURCE, ADC_ULP_MODE_DISABLE);
        

    //     //this->myAdc.init();

    //     // myBinaryLed::binaryLed binaryLed1(3, LEDC_CHANNEL_0, LEDC_TIMER_12_BIT, 0, LEDC_INTR_DISABLE);
    //     // binaryLed1.init();
    // }
}
