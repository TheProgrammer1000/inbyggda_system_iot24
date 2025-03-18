#include <stdio.h>

#include "analog_led.h"
#include "analog_to_digital.h"
#include "binary_led.h"
#include "button.h"
#include "GPIO.h"
#include "led_controller.h"
#include "nvs.h"
#include "printer.h"
#include "esp_sleep.h"

#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "ulp_lp_core.h"
#include "ulp_main.h"

#include "ulp_lp_core.h"  // Ensure this header is included; remove "esp32/ulp.h"

extern "C" {
    extern const uint8_t _binary_ulp_main_bin_start[];
    extern const uint8_t _binary_ulp_main_bin_end[];
}


static void start_ulp_program(void)
{
    esp_err_t err = ulp_lp_core_load_binary(_binary_ulp_main_bin_start, (_binary_ulp_main_bin_end - _binary_ulp_main_bin_start));
    ESP_ERROR_CHECK(err);

    /* Start the program */
    ulp_lp_core_cfg_t cfg = {
        .wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
        .lp_timer_sleep_duration_us = 10000,
    };

    err = ulp_lp_core_run(&cfg);
    ESP_ERROR_CHECK(err);
}




QueueHandle_t xQueue1;


typedef struct analogLedObj_analogToDigitalObj {
    myAnalogLed::analogLed analogLed1;
    adcOneMode::adc adc1;
} analogLedObj_analogToDigitalObj;

void task_movement_sensor_detection(void* pvParameter) {
    
    myGpio::Gpio* gpio6 = (myGpio::Gpio*) pvParameter;
    PRINTF_COLOR(ANSI_BLUE, "gpio pin: %d" NEW_LINE, gpio6->getPin());

    
    xQueue1 = xQueueCreate(10, sizeof(uint32_t));

    int latch = 0;
    int number = 1;

    static TickType_t startTick = xTaskGetTickCount();

    while (1)
    {
   
        int sensorMovementLevel = gpio_get_level((gpio_num_t) gpio6->getPin());

        if(sensorMovementLevel == 0 && latch == 1) {
            latch = 0;
        }
        else if(sensorMovementLevel == 1 && latch == 0){
            //PRINTF_COLOR(ANSI_MAGENTA, "Movement detected!" NEW_LINE);

            startTick = xTaskGetTickCount();

            if(xQueue1 != 0) {
                if(xQueueSend(xQueue1, (void*) &number, (TickType_t) 10) == pdTRUE) {
                    PRINTF_COLOR(ANSI_MAGENTA, "SUCCESSFULLY SENT" NEW_LINE);
                }
                else {
                    PRINTF_COLOR(ANSI_RED, "ERROR" NEW_LINE);
                }
            }
            
            latch = 1;
       }
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void task_analogled(void* pvParameter) {

   analogLedObj_analogToDigitalObj* finalObjects = (analogLedObj_analogToDigitalObj*) pvParameter;


    int dataFromBuffer = 0;

    static bool isMovement = false;
    static TickType_t startTick = xTaskGetTickCount();

    while (1)
    {

        TickType_t elaspedTickSinceStart = xTaskGetTickCount();

        if(elaspedTickSinceStart - startTick >= pdMS_TO_TICKS(7000)) {
            PRINTF_COLOR(ANSI_MAGENTA, "In deep sleep" NEW_LINE);

            ESP_ERROR_CHECK(esp_deep_sleep_try_to_start());
        
            startTick = xTaskGetTickCount();
        }
    
        if(xQueueReceive(xQueue1, (void*) &dataFromBuffer, (TickType_t) 10)  == pdTRUE) {
            PRINTF_COLOR(ANSI_BLUE, "dataFromBuffer: %d" NEW_LINE, dataFromBuffer);
            startTick = xTaskGetTickCount();
            isMovement = true;    
        }
        
        if(isMovement == true) {
            int LDRInputResistans = finalObjects->adc1.getVoltageValueFromLDR();
    
            if(LDRInputResistans != -1) {
                finalObjects->analogLed1.setLed(LDRInputResistans);
                finalObjects->analogLed1.update();
            }    
        }
      

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void onPressedButton(int pin) {
    PRINTF_COLOR(ANSI_BLUE, "BUTTON PRESSED PIN: %d" NEW_LINE, pin);
}


extern "C" void app_main(void)
{
    ledc_timer_config_t ledcTimerConf;

    ledcTimerConf.timer_num = LEDC_TIMER_0;
    ledcTimerConf.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimerConf.freq_hz = 4000;
    ledcTimerConf.duty_resolution = LEDC_TIMER_12_BIT;
    ledcTimerConf.clk_cfg = LEDC_AUTO_CLK;
    ledcTimerConf.deconfigure = false;

    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimerConf));

    myAnalogLed::analogLed analogLed1(5, ledcTimerConf.timer_num, LEDC_CHANNEL_2, LEDC_TIMER_12_BIT, 0b111111111111, LEDC_INTR_DISABLE);    
    adcOneMode::adc adc1(ADC_UNIT_1, ADC_DIGI_CLK_SRC_DEFAULT, ADC_ULP_MODE_DISABLE, ADC_CHANNEL_2, ADC_BITWIDTH_12, ADC_ATTEN_DB_12);
    my_button::button button1(4, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_ENABLE, GPIO_INTR_DISABLE);
    myGpio::Gpio gpio6(6, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_ENABLE, GPIO_INTR_DISABLE);
    
    analogLed1.init();
    adc1.init();
    button1.init();
    gpio6.init();
    
    button1.onPressed_cb = onPressedButton;


    ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup((1UL << button1.get_pin()), ESP_GPIO_WAKEUP_GPIO_HIGH));
    // Enable wakeup from ULP program
    /* Initialize selected GPIO as RTC IO, enable input, disable pullup and pulldown */
    rtc_gpio_init(GPIO_NUM_6);
    rtc_gpio_set_direction(GPIO_NUM_6, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pulldown_dis(GPIO_NUM_6);
    rtc_gpio_pullup_dis(GPIO_NUM_6);

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    /* not a wakeup from ULP, load the firmware */
    start_ulp_program();
    
    if (cause != ESP_SLEEP_WAKEUP_ULP) {
        printf("Not a ULP wakeup, initializing it! \n");
        start_ulp_program();
    }

    printf("Entering in deep sleep\n\n");

    ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());
   
    static TickType_t startTick = xTaskGetTickCount();

    analogLedObj_analogToDigitalObj analogLedObj_analogToDigitalObjFinal = {
        analogLed1,
        adc1
    };

    xTaskCreate(task_movement_sensor_detection, "MovmentSensor", 4096, &gpio6, 2, NULL);
    xTaskCreate(task_analogled, "Analogled", 4096, &analogLedObj_analogToDigitalObjFinal, 3, NULL);

}