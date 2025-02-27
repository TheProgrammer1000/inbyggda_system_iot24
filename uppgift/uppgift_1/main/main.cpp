#include "esp_mac.h"
//#include "nvs.h"
//#include "esp_log.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
//#include "bit_bang.h"
#include "GPIO.h"


extern "C" void app_main(void)
{  
 
    myGpio::Gpio gpioTransmitt(3, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);
    myGpio::Gpio gpioRecieve(2, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_POSEDGE);

    gpioTransmitt.init();
    gpioRecieve.init();

    gpioRecieve.attachInterrupt();

    printf("gpioRecieve level: %d\n",gpio_get_level((gpio_num_t)gpioRecieve.getPin()));

    //gpio_set_level((gpio_num_t)gpioTransmitt.getPin(), 1);
    //vTaskDelay(pdMS_TO_TICKS(1000));
    //gpio_set_level((gpio_num_t)gpioTransmitt.getPin(), 1);



    while(1) {

        vTaskDelay(pdMS_TO_TICKS(30));
    }



    // myBitBang::bitBang bitbang(gpioTransmitt, gpioRecieve);

    // printf("bitbang pin has pin: %d\n", bitbang.getGpioReciever().getPin());


        

    
    // // kolla på om en sekund har gått hur många 1 och 0 har du fått

    // // har du börjat fått en 1 vet du att det är en meddelande igång
    // // sedan resten av 4 bitar där är meddelande sen behöver du inte kolla på resten

    // int arraySize = 9;
    // int arrayInserted[arraySize];

    // int counter = 0;

    // int pulseCounter = 100;
    // bool toggle = false;

    // int level;

    // while(1)
    // {

    //     static TickType_t tickSinceStart = xTaskGetTickCount(); // Spara starttid
    //     TickType_t currentTick = xTaskGetTickCount(); // Uppdatera currentTick varje iteration
    
    //     if(currentTick - tickSinceStart >= pdMS_TO_TICKS(1000)) {

    //         //PRINTF_COLOR(ANSI_BLUE, "HERRE" NEW_LINE);
    //         // tickSinceStart = xTaskGetTickCount(); // Återställ tickSinceStart
    //         // pulseCounter = 100;
    //         gpio_set_level((gpio_num_t)gpioTransmitt.getPin(), 0);

    //         break;
    //     }
    //     else if(currentTick - tickSinceStart >= pdMS_TO_TICKS(pulseCounter) && toggle == false) {
    //         gpio_set_level((gpio_num_t)gpioTransmitt.getPin(), 1);
    //         level = gpio_get_level((gpio_num_t)gpioRecieve.getPin());

    //         PRINTF_COLOR(ANSI_BLUE, "level: %d" NEW_LINE, level);

            
    //         arrayInserted[counter] = level;
    //         counter++;

    //         pulseCounter += 100;

    //         toggle = true;
    //     }
    //     else if(currentTick - tickSinceStart >= pdMS_TO_TICKS(pulseCounter) && toggle == true) {
    //         gpio_set_level((gpio_num_t)gpioTransmitt.getPin(), 0);
    //         level = gpio_get_level((gpio_num_t)gpioRecieve.getPin());
            
    //         PRINTF_COLOR(ANSI_BLUE, "level: %d" NEW_LINE, level);

            
    //         arrayInserted[counter] = level;
    //         counter++;

    //         pulseCounter += 100;

    //         toggle = false;
    //     }
        
    //    //gpioRecieve.update();
    //     vTaskDelay(pdMS_TO_TICKS(30));
    // }

    // for (int i = 0; i < arraySize; i++)
    // {
    //     PRINTF_COLOR(ANSI_BLUE, "arrayInserted[%d]: %d" NEW_LINE, i, arrayInserted[i]);
    // }
    
}
