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

            size_t sizeOfDeviceName;
            size_t sizeOfSerialNumber;

            esp_err_t getDeviceName(char** outDeviceName);
            esp_err_t getSerialNumber(char** outSerialNumber);
            void setDeviceName(char* deviceName); 
            void setSerialNumber (char* serialNumber);

            nvs_handle_t getNvsHandle() { return nvsHandle;}

    };
}
