#include "esp_mac.h"
#include "nvs.h"
//#include "esp_log.h"
#include "analog_to_digital.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"
#include "GPIO.h"
#include "bluetoothLE.h"


extern "C" void app_main(void)
{  


    //  // Initialize NVS.
    //  nvs_flash_init();
    //  // Initialize the NimBLE stack.
    //  ESP_ERROR_CHECK(nimble_port_init());
 
    //  // Set device name and initialize GAP and GATT services.
    //  ESP_ERROR_CHECK(ble_svc_gap_device_name_set("BLE-Server"));
    //  ble_svc_gap_init();
    //  ble_svc_gatt_init();
 
    //  // Initialize our GATT service definitions.
    //  init_gatt_services();
 
    //  // Count and add GATT services.
    //  ble_gatts_count_cfg(gatt_svcs);
    //  ble_gatts_add_svcs(gatt_svcs);
 
    //  // Set the sync callback.
    //  ble_hs_cfg.sync_cb = ble_app_on_sync;
 
    //  // Launch the NimBLE host task.
    //  nimble_port_freertos_init(host_task);
 
        

    while(1)
    {
       
    }

}
