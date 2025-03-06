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
        
    
        switch(nvs_get_str(nvsHandle, KEY_SERIAL_NUMBER, NULL, &this->sizeOfSerialNumber)) {
            case ESP_OK:
                PRINTF_COLOR(ANSI_BLUE, "Retrieved successfully" NEW_LINE);
                this->serialNumber = (char*) malloc(this->sizeOfSerialNumber);
                nvs_get_str(nvsHandle, KEY_SERIAL_NUMBER, serialNumber, &this->sizeOfSerialNumber);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_COLOR(ANSI_RED, "The serial not found on nvs" NEW_LINE);
                break;
            default:
                PRINTF_COLOR(ANSI_RED, "Unhandled error reading nvs" NEW_LINE);
        }


        switch(nvs_get_str(nvsHandle, KEY_DEVICE_NAME, NULL, &this->sizeOfDeviceName)) {
            case ESP_OK:
                this->deviceName = (char*) malloc(this->sizeOfDeviceName);
                PRINTF_COLOR(ANSI_BLUE, "Retrieved successfully" NEW_LINE);
                ESP_ERROR_CHECK(nvs_get_str(nvsHandle, KEY_DEVICE_NAME, deviceName, &this->sizeOfDeviceName));
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
    
    esp_err_t nvs::getDeviceName(char** outDeviceName) {
        if (this->deviceName != NULL) {
            *outDeviceName = this->deviceName;
            return ESP_OK;  // Success
        }
        PRINTF_COLOR(ANSI_YELLOW, "No deviceName to get from NVS" NEW_LINE);
        return ESP_ERR_NOT_FOUND;  // Return error code
    }
    
    esp_err_t nvs::getSerialNumber(char** outSerialNumber) {
        if (this->serialNumber != NULL) {
            *outSerialNumber = this->serialNumber;
            return ESP_OK;  // Success
        }
        PRINTF_COLOR(ANSI_YELLOW, "No serialNumber to get from NVS" NEW_LINE);
        return ESP_ERR_NOT_FOUND;  // Return error code
    }
    

    // Kopiera in nytt device name till arbetsminne och spara på nvs
    void nvs::setDeviceName(char* deviceName) {
        size_t newLen = strlen(deviceName) + 1; // +1 for the null terminator

        this->deviceName = (char*) realloc(this->deviceName, newLen);
        
        if (this->deviceName == NULL) {
            PRINTF_COLOR(ANSI_RED, "Memory allocation failed in setSerialNumber" NEW_LINE);
            return;
        }

        strcpy(this->deviceName, deviceName);

      
        this->sizeOfDeviceName = newLen;
        ESP_ERROR_CHECK(nvs_set_str(this->nvsHandle, KEY_DEVICE_NAME, this->deviceName));
    }

    void nvs::setSerialNumber(char* serialNumber) {

        size_t newLen = strlen(serialNumber) + 1; // +1 for the null terminator
        this->serialNumber = (char*) realloc(this->serialNumber, newLen);
        
        if (this->serialNumber == NULL) {
            PRINTF_COLOR(ANSI_RED, "Memory allocation failed in set serialNumber" NEW_LINE);
            return;
        }

        strcpy(this->serialNumber, serialNumber);

        this->sizeOfSerialNumber = newLen;
        ESP_ERROR_CHECK(nvs_set_str(this->nvsHandle, KEY_SERIAL_NUMBER, this->serialNumber));
    }
}