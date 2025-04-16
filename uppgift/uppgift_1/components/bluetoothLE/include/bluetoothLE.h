#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
//#include "esp_event.h"
#include "printer.h"
#include "nvs_flash.h"
#include <string>
#include <algorithm>

// #include "nimble/nimble_port.h"
// #include "nimble/nimble_port_freertos.h"
// #include "host/ble_hs.h"
// #include "services/gap/ble_svc_gap.h"
// #include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"
#include "binary_led.h"

namespace myBluetooth
{
    class Bluetooth
    {
        uint8_t ble_addr_type = 0;
        const char *TAG = "bluetooth";

        QueueHandle_t xQueue1;
        char dataFromBuffer[255];

        static uint16_t g_conn_handle;
        static uint16_t g_attr_handle_notify;

        // Forward declaration
        static void ble_app_advertise();

        // Callback: Write data to the server (from the client)
        static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
        static int device_read(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
        static int device_notify(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);

        // Representerar en GATT-service men nuvarande 1 service
        static ble_gatt_svc_def gatt_svcs[2];

        // Charactheristics
        static ble_gatt_chr_def characteristics[4];

        void init_gatt_services();

        // BLE event handling callback.
        static int ble_gap_event(struct ble_gap_event *event, void *arg);

        void ble_app_advertise();

        // Callback invoked when the BLE host stack is synchronized.
        void ble_app_on_sync();

        // NimBLE host task running in its own FreeRTOS task.
        void host_task(void *param);
        // ------------BLUETOOTH--END------------------------------
    };
}