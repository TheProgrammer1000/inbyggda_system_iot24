#include <stdio.h>
#include "nvs_flash.h"
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "string.h"


// setting a varible which will be stored in deep-sleep
RTC_DATA_ATTR int storedValue = 0;
/*
    Övning
    *   spara och läs data till/från NVS
    *   spara data i RTC Minne (I.e som finns kvar även om du går in i sleep mode)
    *   Skapa en egen partion där du spara data i

*/

extern "C" void app_main(void)
{
    // const char* myKey = "myKey";

    // ESP_ERROR_CHECK(nvs_flash_init());

    // nvs_handle_t nvsHandle;
    // ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &nvsHandle));


    // nvs_type_t nvsType = NVS_TYPE_I8;

    // // Find if key exist
    // switch (nvs_find_key(nvsHandle, myKey, &nvsType))
    // {
    // case ESP_OK:
    //     PRINTF_COLOR(ANSI_BLUE, "NVS entry for key provided was found" NEW_LINE);
    //     break;
    // case ESP_ERR_NVS_NOT_FOUND: 
    //     PRINTF_COLOR(ANSI_BLUE, "Requested key doesn't exist " NEW_LINE);
    //     break;
    
    // case ESP_ERR_NVS_INVALID_HANDLE:
    //     PRINTF_COLOR(ANSI_BLUE, "handle has been closed or is NULL " NEW_LINE);
    //     break;
    
    // case ESP_FAIL:
    //     PRINTF_COLOR(ANSI_BLUE, "there is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled) - other error codes from the underlying storage driver" NEW_LINE);
    //     break;
    // default:
    //     PRINTF_COLOR(ANSI_BLUE, "Error finding key" NEW_LINE);
    //     break;
    // }

    // int8_t myValue = 0;
    // esp_err_t err = nvs_get_i8(nvsHandle, myKey, &myValue);

    // switch (err) {
    //     case ESP_OK:
    //         printf("Done\n");
    //         printf("myValue = %d" NEW_LINE, myValue);
    //         break;
    //     case ESP_ERR_NVS_NOT_FOUND:
    //         printf("The value is not initialized yet!\n");
    //         break;
    //     default :
    //         printf("Error (%s) reading!\n", esp_err_to_name(err));
    // }

    //myValue++;
    // switch (nvs_set_i8(nvsHandle, myKey, myValue))
    // {
    // case ESP_OK:
    //     PRINTF_COLOR(ANSI_MAGENTA, "Value was set successfully" NEW_LINE);
    //     break;
    // case ESP_FAIL:
    //     PRINTF_COLOR(ANSI_MAGENTA, "Here is an internal error; most likely due to corrupted NVS partition (only if NVS assertion checks are disabled)" NEW_LINE);
    //     break;
    // case ESP_ERR_NVS_READ_ONLY:
    //     PRINTF_COLOR(ANSI_MAGENTA, "Storage handle was opened as read only" NEW_LINE);
    //     break;
    // case ESP_ERR_NVS_INVALID_NAME:
    //     PRINTF_COLOR(ANSI_MAGENTA, "Key name doesn't satisfy constraints" NEW_LINE);
    //     break;

    // case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
    //     PRINTF_COLOR(ANSI_MAGENTA, "There is not enough space in the underlying storage to save the value" NEW_LINE);
    //     break;
    // case ESP_ERR_NVS_REMOVE_FAILED:
    //     PRINTF_COLOR(ANSI_MAGENTA, "The value wasn't updated because flash write operation has failed. The value was written however, and update will be finished after re-initialization of nvs, provided that flash operation doesn't fail again." NEW_LINE);
    //     break;
    // default:
    //     PRINTF_COLOR(ANSI_RED, "Error setting the value" NEW_LINE);
    //     break;
    // }

    // switch(nvs_commit(nvsHandle)) {
    //     case ESP_OK:
    //         PRINTF_GROUP_SUCCESFUL("The changes have been written successfully" NEW_LINE);
    //         break;
    //     case ESP_ERR_NVS_INVALID_HANDLE:
    //         PRINTF_GROUP_FAILED("Handle has been closed or is NULL - other error codes from the underlying storage driver" NEW_LINE);
    //         break;
    // }




    // Deep sleep and variable will be set and retrieve after deep sleep
    // printf("storedValue: %d\n", storedValue);
    // storedValue++;

    // esp_sleep_enable_timer_wakeup(5000000);
    // esp_deep_sleep_start();




    
}