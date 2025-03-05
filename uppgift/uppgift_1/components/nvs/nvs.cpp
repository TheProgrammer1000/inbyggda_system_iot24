#include "nvs.h"




namespace myNvs {
    nvs::nvs() {
        this->nvsHandle = NULL;
    }

    /**
     * @brief default Read/Write
     * 
     */


     /*
        Läs in serialNumber och deviceName till arbetsminne

        om det inte finns
     */
    void nvs::init() {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
        {
            // NVS partition was truncated and needs to be erased
            // Retry nvs_flash_init
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(err);
        ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &this->nvsHandle));

        PRINTF_COLOR(ANSI_BLUE, "Successfully configuered NVS default partition" NEW_LINE);

        if(getSerialNumber() != (char*)"Not found") {
            PRINTF_COLOR(ANSI_BLUE, "Serial number is set" NEW_LINE);

            this->serialNumber = getSerialNumber();
        }
        else {
            PRINTF_COLOR(ANSI_YELLOW, "Serial number is not set!" NEW_LINE);
        }

        if(getDeviceName() != (char*)"Not found") {
            PRINTF_COLOR(ANSI_BLUE, "Device name is set" NEW_LINE);

            this->deviceName = getDeviceName();
        }
        else {
            PRINTF_COLOR(ANSI_YELLOW, "Device name is not set!" NEW_LINE);
        }

        
        ESP_ERROR_CHECK(nvs_close(this->nvsHandle));

    }




    /**
    * @brief 
    *
    */
    
    char* nvs::getDeviceName() {

        ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &this->nvsHandle));
        size_t required_size;

        switch(nvs_get_str(nvsHandle, KEY_DEVICE_NAME, NULL, &required_size)) {
            case ESP_OK:
                this->deviceName = (char*) malloc(required_size);
                PRINTF_COLOR(ANSI_BLUE, "Retrieved successfully" NEW_LINE);
                ESP_ERROR_CHECK(nvs_get_str(nvsHandle, KEY_DEVICE_NAME, deviceName, &required_size));
                return deviceName;  
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_COLOR(ANSI_RED, "The serial not found on nvs" NEW_LINE);
                return "Not found";
                break;

            default:
                PRINTF_COLOR(ANSI_RED, "Unhandled error reading nvs" NEW_LINE);
                return "Not found";
        }

        ESP_ERROR_CHECK(nvs_close(this->nvsHandle));
    }

    // Kopiera in nytt device name till arbetsminne och spara på nvs
    void nvs::setDeviceName(char* deviceName) {

        ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &this->nvsHandle));
        // spara på nvs
        ESP_ERROR_CHECK(nvs_set_str(nvsHandle, KEY_DEVICE_NAME, deviceName));
        PRINTF_COLOR(ANSI_BLUE, "Successfully wrote key/value pair to NVS partition" NEW_LINE);

        ESP_ERROR_CHECK(nvs_close(this->nvsHandle));
    }

    void nvs::setSerialNumber(char* serialNumber) {
        ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &this->nvsHandle));

        ESP_ERROR_CHECK(nvs_set_str(nvsHandle, KEY_SERIAL_NUMBER, serialNumber));
        PRINTF_COLOR(ANSI_BLUE, "Successfully wrote key/value pair to NVS partition" NEW_LINE);

        ESP_ERROR_CHECK(nvs_close(this->nvsHandle));
    }

    char* nvs::getSerialNumber() {

        ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &this->nvsHandle));
        size_t required_size;

        switch(nvs_get_str(nvsHandle, KEY_SERIAL_NUMBER, NULL, &required_size)) {
            case ESP_OK:
                PRINTF_COLOR(ANSI_BLUE, "Retrieved successfully" NEW_LINE);
                this->serialNumber = (char*) malloc(required_size);
                nvs_get_str(nvsHandle, KEY_SERIAL_NUMBER, serialNumber, &required_size);
                return serialNumber;
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_COLOR(ANSI_RED, "The serial not found on nvs" NEW_LINE);
                return "Not found";
                break;
            default:
                PRINTF_COLOR(ANSI_RED, "Unhandled error reading nvs" NEW_LINE);
                return "Not found";
        }

        ESP_ERROR_CHECK(nvs_close(this->nvsHandle));
    }
}