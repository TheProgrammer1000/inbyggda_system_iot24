#pragma once

#include "printer.h"
#include "esp_intr_types.h"
#include "esp_intr_alloc.h"
#include "esp_attr.h"

typedef struct Interrupt_arg {
        volatile int interrupt_flag;
} Interrupt_arg;




namespace interrupts_esp {
    void myInterruptHandler(void* args);
    void init_interrupt(void* args);
}
