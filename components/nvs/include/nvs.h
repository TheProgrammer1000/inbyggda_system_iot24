#pragma once
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_check.h"
#include "printer.h"
#include <string.h>

#define KEY_DEVICE_NAME "keyDevice"
#define KEY_SERIAL_NUMBER "keySerial"

using namespace std;

/*
 xxx init(xxx)
    Ladda alla parametrar från NVS

*/

namespace myNvs {
    class nvs {
        private:
            nvs_handle_t nvsHandle;
            char* deviceName;
            char* serialNumber;
            
        public:
            nvs();
            void init();

            char* getDeviceName();
            char* getSerialNumber();
            void setDeviceName(char* deviceName); 
            void setSerialNumber (char* serialNumber);

            nvs_handle_t getNvsHandle() { return nvsHandle;}

    };
}
