#include <stdio.h>
#include "analog_led.h"
#include "analog_to_digital.h"
#include "binary_led.h"
#include "GPIO.h"
#include "printer.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void task_blink_led(void* pvParameter) {
    myBinaryLed::binaryLed binaryLed1(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);

    binaryLed1.init();
    binaryLed1.blink(2000, 2000);

    while(1)
    {
        binaryLed1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void task_write_serial(void* pvParameter) {    
    while(1)
    {
        //PRINTF_COLOR(ANSI_BLUE, "WRITING SOME" NEW_LINE);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task_potentimeter_to_led(void* pvParameter) {
    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE, ADC_CHANNEL_2, ADC_BITWIDTH_12, ADC_ATTEN_DB_12);
    myAnalogLed::analogLed analogLed1(5, LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE);

    adc1.init();
    analogLed1.init();

    //analogLed1.setLed(4000);

    analogLed1.setLed(2000);

    while(1) {
       // adc1.update();

        //analogLed1.setLed(adc1.getVoltageValueFromLDR());
        analogLed1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}


extern "C" void app_main(void)
{

    //xTaskCreate(task_blink_led, "BlinkLed", 4096, NULL, 2, NULL);

    //xTaskCreate(task_write_serial, "WriteSerial", 4096, NULL, 3, NULL);

    xTaskCreate(task_potentimeter_to_led, "PotentimeterToLed", 4096, NULL, 4, NULL);


}