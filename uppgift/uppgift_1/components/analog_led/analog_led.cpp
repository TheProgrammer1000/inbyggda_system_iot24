#include "analog_led.h"

namespace myAnalogLed {

    void analogLed::init() {
        this->adcInstance.init();
        this->binaryLedInstance.init();
    }

    void analogLed::update() {
        //adc_oneshot_read(this->adcInstance.getAdcUnitHandle(), MY_ADC_CHANNEL, &this->adc_raw_array[0][0]);
        
        //PRINTF_COLOR(ANSI_MAGENTA, "ADC channel: %d and raw data: %d" NEW_LINE, MY_ADC_CHANNEL, this->adc_raw_array[0][0]);

        //ledc_set_duty(binaryLedInstance.getSpeedMode(), binaryLedInstance.getChannel(), this->adc_raw_array[0][0]); 
        //ledc_update_duty(binaryLedInstance.getSpeedMode(), binaryLedInstance.getChannel());
    }
}
