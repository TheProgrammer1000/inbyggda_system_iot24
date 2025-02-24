#include "led_controller.h"


namespace myLedController {
     ledController::ledController() {
        this->analogLeds = NULL;
        this->binaryLeds = NULL;
     }


    //Ta in listor på led komponenter och spara
    void ledController::init(myBinaryLed::binaryLed binaryLeds[], int sizeOfBinary,  myAnalogLed::analogLed analogLeds[], int sizeOfAnalog) {
        this->sizeOfBinary = sizeOfBinary;
        this->sizeOfAnalog = sizeOfAnalog;

        this->binaryLeds = binaryLeds;
        this->analogLeds = analogLeds;

          // Initialize lastWakeTime after the scheduler is running
        this->lastWakeTime = xTaskGetTickCount();
        this->ledState = false; // Assuming LED starts off
    }



    // this was for binary
    void ledController::blinkAll(int milisecLedOn, int milisecLedOff) {

        TickType_t currentTick = xTaskGetTickCount();
        
        if (ledState) {  // LED is currently ON
            if ((currentTick - lastWakeTime) >= pdMS_TO_TICKS(milisecLedOn)) {
                
                if(this->analogLeds != NULL && this->binaryLeds != NULL) {
                    for (int i = 0; i < this->sizeOfAnalog; i++)
                    {
                        analogLeds[i].setLed(0);
                    }

                    for (int i = 0; i < this->sizeOfBinary; i++)
                    {
                        gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 0);
                    }

                }

                ledState = false;
                lastWakeTime = currentTick;

                
            }
        } else {  // LED is currently OFF
            if ((currentTick - lastWakeTime) >= pdMS_TO_TICKS(milisecLedOff)) {
                
                if(this->analogLeds != NULL && this->binaryLeds != NULL) {
                    for (int i = 0; i < this->sizeOfAnalog; i++)
                    {   
                        this->analogLeds[i].setLed(1000);
                    }
                    for (int i = 0; i < this->sizeOfBinary; i++)
                    {
                        gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 1);
                    }
                }
                
                ledState = true;
                lastWakeTime = currentTick;              
            }
        }
    }
}
