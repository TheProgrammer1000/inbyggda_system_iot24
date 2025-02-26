#include "led_controller.h"


namespace myLedController {
     ledController::ledController() {
        this->analogLeds = NULL;
        this->binaryLeds = NULL;

        this->forwardCounter = 0;
        this->reversedCounter = 0;

        this->reversedIndexElementToRemove = 0;
        this->isFirstLap = true;
     }


    //Ta in listor på led komponenter och spara
    void ledController::init(myBinaryLed::binaryLed binaryLeds[], int sizeOfBinary,  myAnalogLed::analogLed analogLeds[], int sizeOfAnalog) {
        this->sizeOfBinary = sizeOfBinary;
        this->sizeOfAnalog = sizeOfAnalog;

        this->binaryLeds = binaryLeds;
        this->analogLeds = analogLeds;

          // Initialize lastWakeTimeOnSnake after the scheduler is running
        this->lastWakeTimeOnSnake = xTaskGetTickCount();
        this->lastWakeTimeOnPeriod = xTaskGetTickCount();

        this->ledState = false; // Assuming LED starts off
    }



    // this was for binary
    void ledController::blinkAll(int milisecLedOn, int milisecLedOff) {
     
        TickType_t currentTick = xTaskGetTickCount();
        
        if (ledState) {  // LED is currently ON
            if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(milisecLedOn)) {
                
                if(this->analogLeds != NULL && this->binaryLeds != NULL) {
                    for (int i = 0; i < this->sizeOfAnalog; i++)
                    {   
                        ledc_set_duty(analogLeds[i].getSpeedMode(), analogLeds[i].getChannel(), 0); // sätter 1000 hårdkodat
                        ledc_update_duty(analogLeds[i].getSpeedMode(), analogLeds[i].getChannel());
                    }

                    for (int i = 0; i < this->sizeOfBinary; i++)
                    {
                        gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 0);
                    }

                }

                ledState = false;
                lastWakeTimeOnSnake = currentTick;

                
            }
        } else {  // LED is currently OFF
            if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(milisecLedOff)) {
                
                if(this->analogLeds != NULL && this->binaryLeds != NULL) {
                    for (int i = 0; i < this->sizeOfAnalog; i++)
                    {   
                        ledc_set_duty(analogLeds[i].getSpeedMode(), analogLeds[i].getChannel(), 1000); // sätter 1000 hårdkodat
                        ledc_update_duty(analogLeds[i].getSpeedMode(), analogLeds[i].getChannel());
                    }
                    for (int i = 0; i < this->sizeOfBinary; i++)
                    {
                        gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 1);
                    }
                }
                
                ledState = true;
                lastWakeTimeOnSnake = currentTick;              
            }
        }
    }

    void ledController::myLedAnimation(int lapPeriodMs, int sizeOfBinary, bool isAnalog) {

        TickType_t currentTick = xTaskGetTickCount();

        static int counter = 0;
        static bool isAllSet = false;

        if(isAnalog == true) {
            if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(lapPeriodMs)) { 

                PRINTF_COLOR(ANSI_BLUE,"HERRE" NEW_LINE);

                lastWakeTimeOnSnake = xTaskGetTickCount();
            }
        }
        else {
            if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(lapPeriodMs)) { 
                if(counter >= sizeOfBinary) {
                    isAllSet = true;
                }  
    
                if(isAllSet == true) {
                    counter--;
                    int arrayIndex = counter % sizeOfBinary;
                    gpio_set_level((gpio_num_t)this->binaryLeds[arrayIndex].getPin(), 0);
    
                    lastWakeTimeOnSnake = xTaskGetTickCount();
    
                    if(counter <= 0) {
                        isAllSet = false;
                    }
                }
                else {
                    int arrayIndex = counter % sizeOfBinary;
    
                    gpio_set_level((gpio_num_t)this->binaryLeds[arrayIndex].getPin(), 1);
                    counter++;
        
                    lastWakeTimeOnSnake = xTaskGetTickCount();
                }
            }
        }
    }

    void ledController::snakeFushionAnimation(int length, int lapPeriodMs, int sizeOfLed) {
       static int counter = 0;
       static bool isSecoundCycle = false;
       static bool isLastCycle = false;

       TickType_t currentTick = xTaskGetTickCount();

       if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(lapPeriodMs)) { 
            int arrayIndex = counter % sizeOfBinary;

            if(isLastCycle == true) {
                int tempIndexArray = counter % sizeOfAnalog - 1;
                for (int i = 0; i < length; i++)
                {
                    if(tempIndexArray < 0) {
                        tempIndexArray = sizeOfBinary - 1;
                    }
                    gpio_set_level((gpio_num_t)this->binaryLeds[tempIndexArray].getPin(), 0);
                    
                    tempIndexArray--;
                }
                isLastCycle = false;
            }
            
            else if(counter >= length && isSecoundCycle == true && isLastCycle == false) {
                for (int i = counter; i >= 0; i--)
                {
                    gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 0);
                }
                isSecoundCycle = false;
            }

            if(counter % sizeOfLed >= 0 && (counter + length) >= sizeOfLed) {
                int indexOfArray = counter % sizeOfAnalog;

                for (int i = 0; i < length; i++)
                {
                    gpio_set_level((gpio_num_t)this->binaryLeds[indexOfArray].getPin(), 1);
                    
                    counter++;
                    indexOfArray = counter % sizeOfLed;
                }
                counter = indexOfArray;
                isLastCycle = true;
            }
            else if(counter < sizeOfLed) {
                for (int i = 0; i < length; i++)
                {
                    gpio_set_level((gpio_num_t)this->binaryLeds[counter].getPin(), 1);
                    counter++;
                }
                isSecoundCycle = true;     
            }

            lastWakeTimeOnSnake = xTaskGetTickCount();
        }
    }

    void ledController::snakeAnimation(int length, int lapPeriodMs, int sizeOfLed, bool reverse) {
        TickType_t currentTick = xTaskGetTickCount();

        if(reverse == false) {
            if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(lapPeriodMs)) { 
            
                if(this->isFirstLap == true) {
                    for (int i = 0; i < length; i++)
                    {
                        gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 1);
                        this->forwardCounter++;
                    }
                    this->isFirstLap = false;
                }
                else {
        
                    int indexOfInsertLed = this->forwardCounter % sizeOfLed;
                    int indexOfRemoveLed = (this->forwardCounter - length) % sizeOfLed;
    
                    gpio_set_level((gpio_num_t)this->binaryLeds[indexOfInsertLed].getPin(), 1);
                    gpio_set_level((gpio_num_t)this->binaryLeds[indexOfRemoveLed].getPin(), 0);
        
                    this->forwardCounter++;
                }
                lastWakeTimeOnSnake = xTaskGetTickCount();
            }
        }
        else {
            if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(lapPeriodMs)) { 
                  if(this->isFirstLap == true) {
                        int indexOfLastLed = sizeOfLed - 1;
                        for (int i = 0; i < length; i++)
                        {
                            gpio_set_level((gpio_num_t)this->binaryLeds[indexOfLastLed].getPin(), 1);
                            //counter++;
                            indexOfLastLed--;
                        }
                        this->isFirstLap = false;

                        this->reversedIndexElementToRemove = sizeOfLed - 1;
                        this->reversedCounter = indexOfLastLed;
                    }
                    else {
                        // counter
                        if(this->reversedCounter < 0) {
                            this->reversedCounter = sizeOfLed - 1;
                        }

                        int indexOfInsertLed = this->reversedCounter % sizeOfLed;
                        int indexOfRemoveLed = (this->reversedIndexElementToRemove + this->reversedCounter + (length + 1)) % sizeOfLed;

                        gpio_set_level((gpio_num_t)this->binaryLeds[indexOfInsertLed].getPin(), 1);
                        gpio_set_level((gpio_num_t)this->binaryLeds[indexOfRemoveLed].getPin(), 0);

                        this->reversedCounter--;

                    }

                lastWakeTimeOnSnake = xTaskGetTickCount();
            }
        }
    }
}
