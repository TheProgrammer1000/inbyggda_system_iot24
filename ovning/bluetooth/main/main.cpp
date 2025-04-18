#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"
#include "printer.h"
#include "nvs_flash.h"
#include <string>
#include <algorithm>

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"
#include "binary_led.h"
#include "analog_led.h"


static constexpr const char* TAG = "BLE-Server";
uint8_t ble_addr_type = 0;

QueueHandle_t xQueue1;
char dataFromBuffer[255];

static uint16_t g_conn_handle;
static uint16_t g_attr_handle_notify;

// Forward declaration
void ble_app_advertise();

// Callback: Write data to the server (from the client)
static int recieve_data_from_client(uint16_t conn_handle, uint16_t attr_handle,
                        struct ble_gatt_access_ctxt* ctxt, void* arg) {

    PRINTF_COLOR(ANSI_MAGENTA, "Data from the client: %.*s" NEW_LINE, ctxt->om->om_len, ctxt->om->om_data);
    fflush(stdout); // Ensure the colored output is immediately flushed to terminal


    size_t data_len = ctxt->om->om_len;  // number of bytes received
    char buf[data_len + 1];              // allocate space for data + null terminator

    // Use memcpy to copy exactly data_len bytes
    memcpy(buf, ctxt->om->om_data, data_len);
    buf[data_len] = '\0';   


    if(xQueueSend(xQueue1, buf, (TickType_t)4) == pdTRUE) {
        PRINTF_COLOR(ANSI_BLUE, "Successfully posted" NEW_LINE);
    }

    return 0;
}

// Callback: Read data from the server (to the client)
static int send_to_client(uint16_t conn_handle, uint16_t attr_handle,
                       struct ble_gatt_access_ctxt* ctxt, void* arg) {
    const char* msg = "Data from the server";
    os_mbuf_append(ctxt->om, msg, std::strlen(msg));
    return 0;
}

static int device_notify(uint16_t conn_handle, uint16_t attr_handle,
    struct ble_gatt_access_ctxt* ctxt, void* arg) {

        PRINTF_COLOR(ANSI_BLUE, "Successfully posted" NEW_LINE);

        return 0;
}


// Representerar en GATT-service men nuvarande 1 service
static ble_gatt_svc_def gatt_svcs[2];

// Charactheristics 
static ble_gatt_chr_def characteristics[4];

static void init_gatt_services() {
    // Create persistent UUID constants so we avoid taking the address of rvalues.
    static const ble_uuid16_t uuid_service = BLE_UUID16_INIT(0x180);
    static const ble_uuid16_t uuid_read    = BLE_UUID16_INIT(0xFEF4);
    static const ble_uuid16_t uuid_write   = BLE_UUID16_INIT(0xDEAD);
    static const ble_uuid16_t uuid_notify = BLE_UUID16_INIT(0xBEEF);


    std::memset(gatt_svcs, 0, sizeof(gatt_svcs));
    gatt_svcs[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
    gatt_svcs[0].uuid = (ble_uuid_t*)&uuid_service;

    std::memset(characteristics, 0, sizeof(characteristics));
    characteristics[0].uuid = (ble_uuid_t*)&uuid_read;
    characteristics[0].flags = BLE_GATT_CHR_F_READ;
    characteristics[0].access_cb = send_to_client;
    
    characteristics[1].uuid = (ble_uuid_t*)&uuid_write;
    characteristics[1].flags = BLE_GATT_CHR_F_WRITE;
    characteristics[1].access_cb = recieve_data_from_client;

    characteristics[2].uuid = (ble_uuid_t*)&uuid_notify;
    characteristics[2].flags = BLE_GATT_CHR_F_NOTIFY;
    characteristics[2].access_cb = device_notify;

    characteristics[2].val_handle = &g_attr_handle_notify;
    // characteristics[2].val_handle = &g_attr_handle_notify;

    gatt_svcs[0].characteristics = characteristics;

}

// BLE event handling callback.
static int ble_gap_event(struct ble_gap_event* event, void* arg) {
    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status != 0) {
                // Connection failed; restart advertising.
                PRINTF_COLOR(ANSI_BLUE, "BLE GAP EVENT CONNECT FAILED" NEW_LINE);
                ble_app_advertise();
            } else {
                g_conn_handle = event->connect.conn_handle;
                PRINTF_COLOR(ANSI_BLUE, "BLE GAP EVENT CONNECT OK!" NEW_LINE);
            }
            break;

        case BLE_GAP_EVENT_ADV_COMPLETE:
            
            PRINTF_COLOR(ANSI_BLUE, "BLE GAP EVENT ADV COMPLETE");
            ble_app_advertise();
            break;

        default:
            break;
    }
    return 0;
}

void ble_app_advertise() {
    struct ble_hs_adv_fields fields;
    std::memset(&fields, 0, sizeof(fields));

    const char* device_name = ble_svc_gap_device_name();
    fields.name = reinterpret_cast<uint8_t*>(const_cast<char*>(device_name));
    fields.name_len = std::strlen(device_name);
    fields.name_is_complete = 1;
    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "ble_gap_adv_set_fields failed; rc=%d", rc);
    }

    struct ble_gap_adv_params adv_params;
    std::memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(ble_addr_type, nullptr, BLE_HS_FOREVER, &adv_params,
                           ble_gap_event, nullptr);
    if (rc != 0) {
        ESP_LOGE(TAG, "ble_gap_adv_start failed; rc=%d", rc);
    }
}

// Callback invoked when the BLE host stack is synchronized.
void ble_app_on_sync() {
    ble_hs_id_infer_auto(0, &ble_addr_type);
    ble_app_advertise();
}

// NimBLE host task running in its own FreeRTOS task.
extern "C" void host_task(void* param) {
    nimble_port_run();
}
// ------------BLUETOOTH--END------------------------------


void task_recieve_message(void *pvParameter) {

    myBinaryLed::binaryLed* binaryLed1 = (myBinaryLed::binaryLed*)pvParameter;

    while(1) {

        if(xQueueReceive(xQueue1, (void*) &dataFromBuffer, (TickType_t) 10)  == pdTRUE) {
            PRINTF_COLOR(ANSI_BLUE, "dataFromBuffer: %s" NEW_LINE, dataFromBuffer);

            std::string textRecieved = (std::string) dataFromBuffer;
            
            std::transform(textRecieved.begin(), textRecieved.end(), textRecieved.begin(),
                [](unsigned char c){ return std::tolower(c); });

        
            if(textRecieved == "on") {
                PRINTF_COLOR(ANSI_BLUE, "TURN ON THE LED" NEW_LINE);

                const char* myData = "Hello from ESP32!";
                struct os_mbuf* om = ble_hs_mbuf_from_flat(myData, strlen(myData));
                int rc = ble_gatts_notify_custom(g_conn_handle, g_attr_handle_notify, om);
                if (rc != 0) {
                    ESP_LOGE(TAG, "Failed to notify, rc=%d", rc);
                }

                gpio_set_level(gpio_num_t(binaryLed1->getPin()), 1);
           
            }

            if(textRecieved == "off") {
                PRINTF_COLOR(ANSI_BLUE, "TURN off THE LED" NEW_LINE);
                gpio_set_level(gpio_num_t(binaryLed1->getPin()), 0);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}



// Main entry point for the ESP-IDF application.
extern "C" void app_main() {
    myBinaryLed::binaryLed* binaryLed1 = new myBinaryLed::binaryLed(3, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_ENABLE, GPIO_INTR_DISABLE);
    binaryLed1->init();

    ledc_timer_config_t ledcTimerConf;
    ledcTimerConf.freq_hz = 4000;
    ledcTimerConf.clk_cfg = LEDC_AUTO_CLK;
    ledcTimerConf.duty_resolution = LEDC_TIMER_12_BIT;
    ledcTimerConf.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimerConf.timer_num = LEDC_TIMER_0;
    //ledcTimerConf.deconfigure = false;
    ledcTimerConf.deconfigure = false;

    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimerConf));

    myAnalogLed::analogLed* analogLed1 = new myAnalogLed::analogLed(6,  ledcTimerConf.timer_num, LEDC_CHANNEL_0, ledcTimerConf.duty_resolution, 0b111111111111, LEDC_INTR_DISABLE);

    analogLed1->init();
    analogLed1->setLed(4095);
    analogLed1->update();

    // // Initialize NVS.
    // nvs_flash_init();
    // // Initialize the NimBLE stack.
    // ESP_ERROR_CHECK(nimble_port_init());

    // // Set device name and initialize GAP and GATT services.
    // ESP_ERROR_CHECK(ble_svc_gap_device_name_set("BLE-Server"));
    // ble_svc_gap_init();
    // ble_svc_gatt_init();

    // // Initialize our GATT service definitions.
    // init_gatt_services();

    // // Count and add GATT services.
    // ble_gatts_count_cfg(gatt_svcs);
    // ble_gatts_add_svcs(gatt_svcs);

    // // Set the sync callback.
    // ble_hs_cfg.sync_cb = ble_app_on_sync;

    // // Launch the NimBLE host task.
    // nimble_port_freertos_init(host_task);

    // xQueue1 = xQueueCreate(10, sizeof(uint_least64_t));

    // xTaskCreate(task_recieve_message, "taskRecieveCommand", 4096, (void*)binaryLed1, 2, NULL);
}