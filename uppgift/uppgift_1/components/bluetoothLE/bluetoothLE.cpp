#include <stdio.h>
#include "bluetoothLE.h"


namespace myBluetooth
{
    void Bluetooth::init_gatt_services()
    {
        // Create persistent UUID constants so we avoid taking the address of rvalues.
        static const ble_uuid16_t uuid_service = BLE_UUID16_INIT(0x180);
        static const ble_uuid16_t uuid_read = BLE_UUID16_INIT(0xFEF4);
        static const ble_uuid16_t uuid_write = BLE_UUID16_INIT(0xDEAD);
        static const ble_uuid16_t uuid_notify = BLE_UUID16_INIT(0xBEEF);

        std::memset(gatt_svcs, 0, sizeof(gatt_svcs));
        gatt_svcs[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
        gatt_svcs[0].uuid = (ble_uuid_t *)&uuid_service;

        std::memset(characteristics, 0, sizeof(characteristics));
        characteristics[0].uuid = (ble_uuid_t *)&uuid_read;
        characteristics[0].flags = BLE_GATT_CHR_F_READ;
        characteristics[0].access_cb = send_to_client;

        characteristics[1].uuid = (ble_uuid_t *)&uuid_write;
        characteristics[1].flags = BLE_GATT_CHR_F_WRITE;
        characteristics[1].access_cb = recieve_data_from_client;

        characteristics[2].uuid = (ble_uuid_t *)&uuid_notify;
        characteristics[2].flags = BLE_GATT_CHR_F_NOTIFY;
        characteristics[2].access_cb = device_notify;

        characteristics[2].val_handle = &g_attr_handle_notify;
        // characteristics[2].val_handle = &g_attr_handle_notify;

        gatt_svcs[0].characteristics = characteristics;
    }
    
    int Bluetooth::ble_gap_event(struct ble_gap_event *event, void *arg)
    {
        switch (event->type)
        {
        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status != 0)
            {
                PRINTF_COLOR(ANSI_BLUE, "BLE GAP EVENT CONNECT FAILED" NEW_LINE);
                ble_app_advertise();
            }
            else
            {
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

    void Bluetooth::ble_app_advertise()
    {
        struct ble_hs_adv_fields fields;
        std::memset(&fields, 0, sizeof(fields));

        const char *device_name = ble_svc_gap_device_name();
        fields.name = reinterpret_cast<uint8_t *>(const_cast<char *>(device_name));
        fields.name_len = std::strlen(device_name);
        fields.name_is_complete = 1;
        int rc = ble_gap_adv_set_fields(&fields);
        if (rc != 0)
        {
            PRINTF_COLOR(ANSI_BLUE, "FAILED WITH BLE_APP_ADVERTISE");
        }

        struct ble_gap_adv_params adv_params;
        std::memset(&adv_params, 0, sizeof(adv_params));
        adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
        adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

        rc = ble_gap_adv_start(0, nullptr, BLE_HS_FOREVER, &adv_params, ble_gap_event, nullptr);

        if (rc != 0)
        {
            PRINTF_COLOR(ANSI_BLUE, "ble_gap_adv_start failed; rc=%d", rc);
        }
    }

    void Bluetooth::ble_app_on_sync()
    {
        ble_hs_id_infer_auto(0, &ble_addr_type);
        ble_app_advertise();
    }

    void Bluetooth::host_task(void *param)
    {
        nimble_port_run();
    }

    int Bluetooth::recieve_data_from_client(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
    {

        PRINTF_COLOR(ANSI_MAGENTA, "Data from the client: %.*s" NEW_LINE, ctxt->om->om_len, ctxt->om->om_data);
        fflush(stdout); // Ensure the colored output is immediately flushed to terminal

        size_t data_len = ctxt->om->om_len; // number of bytes received
        char buf[data_len + 1];             // allocate space for data + null terminator

        // Use memcpy to copy exactly data_len bytes
        memcpy(buf, ctxt->om->om_data, data_len);
        buf[data_len] = '\0';

        // if (xQueueSend(xQueue1, buf, (TickType_t)4) == pdTRUE)
        // {
        //     PRINTF_COLOR(ANSI_BLUE, "Successfully posted" NEW_LINE);
        // }

        return 0;
    }

    int Bluetooth::send_to_client(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
    {
        const char *msg = "Data from the server";
        os_mbuf_append(ctxt->om, msg, std::strlen(msg));
        return 0;
    }

    int Bluetooth::device_notify(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
    {
        PRINTF_COLOR(ANSI_BLUE, "Successfully posted" NEW_LINE);
        return 0;
    }
}