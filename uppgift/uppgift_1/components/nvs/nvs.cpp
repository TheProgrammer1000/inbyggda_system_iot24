#include "nvs.h"




namespace myNvs {
    nvs::nvs() {
        this->nvsHandle = NULL;
        this->deviceName = NULL;
        this->serialNumber = NULL;
    }

    /**
     * @brief default Read/Write
     * 
     */


     /*
        Läs in serialNumber och deviceName till arbetsminne
        Om det inte finns så får användaren sätta dessa och göra init igen
        
     */
    void nvs::init() {

        //ESP_ERROR_CHECK(nvs_flash_erase());
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



        ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &this->nvsHandle));
        
        size_t required_size;
        switch(nvs_get_str(nvsHandle, KEY_SERIAL_NUMBER, NULL, &required_size)) {
            case ESP_OK:
                PRINTF_COLOR(ANSI_BLUE, "Retrieved successfully" NEW_LINE);
                this->serialNumber = (char*) malloc(required_size);
                nvs_get_str(nvsHandle, KEY_SERIAL_NUMBER, serialNumber, &required_size);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_COLOR(ANSI_RED, "The serial not found on nvs" NEW_LINE);
                break;
            default:
                PRINTF_COLOR(ANSI_RED, "Unhandled error reading nvs" NEW_LINE);
        }


        size_t required_size_2;
        switch(nvs_get_str(nvsHandle, KEY_DEVICE_NAME, NULL, &required_size_2)) {
            case ESP_OK:
                this->deviceName = (char*) malloc(required_size_2);
                PRINTF_COLOR(ANSI_BLUE, "Retrieved successfully" NEW_LINE);
                ESP_ERROR_CHECK(nvs_get_str(nvsHandle, KEY_DEVICE_NAME, deviceName, &required_size_2));
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_COLOR(ANSI_RED, "The serial not found on nvs" NEW_LINE);
                break;

            default:
                PRINTF_COLOR(ANSI_RED, "Unhandled error reading nvs" NEW_LINE);
                break;
        }
    }




    /**
    * @brief 
    *
    * @attention bara returna deviceName
    */
    
    char* nvs::getDeviceName() {
        return this->deviceName;
    }

    // Bara hämta serialNumber!
    char* nvs::getSerialNumber() {
        return this->serialNumber;
    }

    // Kopiera in nytt device name till arbetsminne och spara på nvs
    void nvs::setDeviceName(char* deviceName) {
        
        // spara på nvs
        ESP_ERROR_CHECK(nvs_set_str(this->nvsHandle, KEY_DEVICE_NAME, deviceName));
        PRINTF_COLOR(ANSI_BLUE, "Successfully wrote key/value pair to NVS partition" NEW_LINE);

        this->deviceName = deviceName;
    }

    void nvs::setSerialNumber(char* serialNumber) {

        ESP_ERROR_CHECK(nvs_set_str(this->nvsHandle, KEY_SERIAL_NUMBER, serialNumber));
        PRINTF_COLOR(ANSI_BLUE, "Successfully wrote key/value pair to NVS partition" NEW_LINE);

        this->serialNumber = serialNumber;
    }
}