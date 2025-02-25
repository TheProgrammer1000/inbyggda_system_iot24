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

    void ledController::snakeAnimation(int length, int lapPeriodMs, int sizeOfLed) {
        /*
            length: längden på snaken
            lapPeriodMs: hur snabbt varje varv ska gå
        */
       static int counter = 0;
       static bool isFirst = false;

       static int removeLed = 0;

       TickType_t currentTick = xTaskGetTickCount();

       if ((currentTick - lastWakeTimeOnSnake) >= pdMS_TO_TICKS(lapPeriodMs)) { 
            //PRINTF_COLOR(ANSI_BLUE,"HERRE" NEW_LINE);

            int arrayIndex = counter % sizeOfBinary;

            printf("arrayIndex: %d \n", arrayIndex);

            if(counter >= length - 1) {
      
                for (int i = counter; i >= 0; i--)
                {
                    /* code */
                    PRINTF_COLOR(ANSI_BLUE, "DELETE LED: %d" NEW_LINE, counter);
                    gpio_set_level((gpio_num_t)this->binaryLeds[i].getPin(), 0);
                }
                

            }

            for (int i = 0; i < length; i++)
            {
                gpio_set_level((gpio_num_t)this->binaryLeds[counter].getPin(), 1);
                counter++;


                printf("counter: %d \n", counter);
            }
            
    
          
            //gpio_set_level((gpio_num_t)this->binaryLeds[arrayIndex + 1].getPin(), 1);
            
            
            //counter += 2;

            lastWakeTimeOnSnake = xTaskGetTickCount();
        }
    }
}
