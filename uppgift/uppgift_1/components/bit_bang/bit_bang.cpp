#include "bit_bang.h"

namespace myBitBang {


    /**
     * Skicka data en 1 som indikerar att nu komemr ett meddelande
     * efter denna bit som är 1 kommer meddlande på 4 bitar!
     *  
     * Ha en timer en takt som går en gång i sekunden och lyssnar på 
     * meddalande som kommer in
     * 
     */

    // void bitBang::init(myGpio::Gpio gpioTransmitter, myGpio::Gpio gpioReciever) {

       
    // }
    void bitBang::init(int baudRate) {
        this->baudRate = baudRate;
        this->level = 0;
        this->isHighOrLow = false;
    }

    void bitBang::update() {
        this->tickSinceStart = xTaskGetTickCount(); // Spara starttid
        TickType_t currentTick = xTaskGetTickCount(); // Uppdatera currentTick varje iteration

        
        
        // if(currentTick - tickSinceStart >= pdMS_TO_TICKS(baudRate)) {

        //     //PRINTF_COLOR(ANSI_BLUE, "HERRE" NEW_LINE);
        //     // tickSinceStart = xTaskGetTickCount(); // Återställ tickSinceStart
        //     // pulseCounter = 100;
        //     gpio_set_level((gpio_num_t)gpioTransmitter.getPin(), 0);

        // }
    
    }
}