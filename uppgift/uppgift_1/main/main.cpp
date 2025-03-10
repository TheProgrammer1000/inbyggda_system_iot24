#include "esp_mac.h"
#include "nvs.h"
//#include "esp_log.h"
#include "analog_to_digital.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"
#include "GPIO.h"


extern "C" void app_main(void)
{  

    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE, ADC_CHANNEL_2, ADC_BITWIDTH_12, ADC_ATTEN_DB_12);

    adc1.init();

    while(1) {


        int voltageValue = adc1.getVoltageValueFromLDR();
        if(voltageValue != -1) {
            PRINTF_COLOR(ANSI_BLUE, "voltageValue: %d" NEW_LINE, voltageValue);
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }

}
