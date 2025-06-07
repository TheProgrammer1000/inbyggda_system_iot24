#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "BLE_CLIENT";
static uint8_t own_addr_type;
static uint16_t conn_handle;
static uint16_t char_handle; // Store value handle here

// Define the 16-bit UUIDs
static const uint16_t SERVICE_UUID = 0x1111;
static const uint16_t CHAR_UUID = 0x2222;

static bool should_connect = true;

bool parse_adv_name(const uint8_t *adv_data, uint8_t adv_data_len, std::string &name_out)
{
    uint8_t index = 0;
    while (index < adv_data_len)
    {
        uint8_t length = adv_data[index++];
        if (length == 0)
            break;
        uint8_t type = adv_data[index];
        if (type == 0x09)
        { // Complete Local Name
            name_out.assign((const char *)(adv_data + index + 1), length - 1);
            return true;
        }
        index += length;
    }
    return false;
}

int write_to_characteristic(const std::string &message)
{
    int rc = ble_gattc_write_flat(conn_handle, char_handle, message.data(), message.size(), NULL, NULL);
    if (rc == 0)
    {
        ESP_LOGI(TAG, "✅ Successfully wrote: %s", message.c_str());
    }
    else
    {
        ESP_LOGE(TAG, "❌ Failed to write characteristic; rc=%d", rc);
    }
    return rc;
}

int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_DISC:
    {
        std::string name;
        if (parse_adv_name(event->disc.data, event->disc.length_data, name))
        {
            ESP_LOGI(TAG, "Discovered name: %s", name.c_str());
            if (should_connect && name == "dennistesting")
            {
                should_connect = false;
                ESP_LOGI(TAG, "Target found, connecting...");
                ble_gap_disc_cancel();
                int rc = ble_gap_connect(own_addr_type, &event->disc.addr, BLE_HS_FOREVER, NULL, ble_gap_event, NULL);
                if (rc != 0)
                {
                    ESP_LOGE(TAG, "Connection failed; rc=%d", rc);
                }
            }
        }
        return 0;
    }

    case BLE_GAP_EVENT_CONNECT:
    {
        if (event->connect.status == 0)
        {

            ESP_LOGI(TAG, "Connected to device");
            conn_handle = event->connect.conn_handle;

            // Discover service
            ble_uuid16_t svc_uuid = {BLE_UUID_TYPE_16, SERVICE_UUID};
            int rc = ble_gattc_disc_svc_by_uuid(conn_handle, &svc_uuid.u, [](uint16_t conn_handle, const struct ble_gatt_error *error, const struct ble_gatt_svc *svc, void *arg) -> int
                                                {
                            if (error->status != 0) {
                                ESP_LOGE(TAG, "Service discovery failed: %d", error->status);
                                return 0;
                            }
                            ESP_LOGI(TAG, "✅ Found service with handle range: %d - %d", svc->start_handle, svc->end_handle);

                            // Now discover the characteristic
                            ble_uuid16_t chr_uuid = {BLE_UUID_TYPE_16, CHAR_UUID};
                            int rc = ble_gattc_disc_chrs_by_uuid(
                                conn_handle,
                                svc->start_handle, svc->end_handle,
                                &chr_uuid.u,
                                [](uint16_t conn_handle, const struct ble_gatt_error *error,
                                const struct ble_gatt_chr *chr, void *arg) -> int {
                                    if (error->status != 0) {
                                        ESP_LOGE(TAG, "Characteristic discovery failed: %d", error->status);
                                        return 0;
                                    }
                                    ESP_LOGI(TAG, "✅ Found characteristic: val_handle=%d", chr->val_handle);
                                    char_handle = chr->val_handle;
                                    write_to_characteristic("Hello from ESP32!");
                                    return 0;
                                },
                                NULL
                            );
                            return rc; }, NULL);
            return rc;
        }
        else
        {
            ESP_LOGE(TAG, "Connection failed; status=%d", event->connect.status);
        }
        return 0;
    }

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "Disconnected from device; reason=%d", event->disconnect.reason);
        return 0;

    default:
        return 0;
    }
}

void ble_app_on_sync(void)
{
    int rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "Address type inference failed; rc=%d", rc);
        return;
    }

    struct ble_gap_disc_params disc_params = {};
    disc_params.itvl = 0x0010;
    disc_params.window = 0x0010;
    disc_params.filter_policy = 0;
    disc_params.passive = 0;
    disc_params.limited = 0;

    ESP_LOGI(TAG, "Starting scan...");
    rc = ble_gap_disc(own_addr_type, 30000, &disc_params, ble_gap_event, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "Failed to start scan; rc=%d", rc);
    }
}

void ble_app_on_reset(int reason)
{
    ESP_LOGE(TAG, "Resetting BLE stack; reason=%d", reason);
}

void host_task(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

extern "C" void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    nimble_port_init();

    ble_hs_cfg.sync_cb = ble_app_on_sync;
    ble_hs_cfg.reset_cb = ble_app_on_reset;

    nimble_port_freertos_init(host_task);
}
