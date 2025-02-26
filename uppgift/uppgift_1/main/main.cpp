#include "esp_mac.h"
//#include "nvs.h"
//#include "esp_log.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"
#include "GPIO.h"


extern "C" void app_main(void)
{  
 
    myGpio::Gpio gpioTransmitt(3, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);
    myGpio::Gpio gpioRecieve(2, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);

    gpioTransmitt.init();
    gpioRecieve.init();

    
    // kolla på om en sekund har gått hur många 1 och 0 har du fått

    // har du börjat fått en 1 vet du att det är en meddelande igång
    // sedan resten av 4 bitar där är meddelande sen behöver du inte kolla på resten

    while(1)
    {
       gpioRecieve.update();
       vTaskDelay(30);
    }
}
