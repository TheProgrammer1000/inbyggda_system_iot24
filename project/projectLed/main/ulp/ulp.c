// ulp/ulp.c
#include "ulp_lp_core.h"  // or include any header that provides low-level access for ULP
#include <stdbool.h>
#include "ulp_lp_core.h"
#include "ulp_lp_core_utils.h"
#include "ulp_lp_core_gpio.h"

/* this variable will be exported as a public symbol, visible from main CPU: */
bool gpio_level_previous = false;

// Define the RTC IO number corresponding to your input pin
#define WAKEUP_PIN LP_IO_NUM_6

int main (void)
{
    while(1) {
        bool gpio_level = (bool)ulp_lp_core_gpio_get_level(WAKEUP_PIN);
        /* Debounce the input, only trigger a wakeup if the changed value is stable for DEBOUNCE_SAMPLES samples */
        if(gpio_level == true) {
            ulp_lp_core_wakeup_main_processor();
            /* this variable will be exported as a public symbol, visible from main CPU: */
            gpio_level_previous = gpio_level;
            return 0;
        }
        /* ulp_lp_core_halt() is called automatically when main exits */    
    }
    return 0;
}