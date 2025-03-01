#include "esp_mac.h"
#include "printer.h"
#include "analog_led.h"
#include "binary_led.h"
#include "nvs.h"
#include "potentiometer.h"
#include "button.h"
#include "led_controller.h"
#include "GPIO.h"


extern "C" void app_main(void)
{
    PRINTF_COLOR(ANSI_BLUE, "HELLO WORLD! " NEW_LINE); 
    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE);
    
    adc1.init();
}