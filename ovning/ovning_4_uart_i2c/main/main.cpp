#include <stdio.h>
#include "driver/uart.h"
#include <string.h>
#include "esp_check.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define UART_PIN_RECIEVE GPIO_NUM_3
#define UART_PIN_TRANSMITT GPIO_NUM_11

#define SEND_UART_PERIOD 2000
#define RECIEVE_WAIT_UART 30
 

#define UART_RECIEVE_BUFFER_SIZE 128

const char* TAG = "MAIN";


extern "C" void app_main(void)
{

    const uart_port_t uart_num = UART_NUM_1;
    uart_config_t my_uart_config;


    const int rx_buffer_size = (1024 * 2);
    const int tx_buffer_size = 512;


    // inita kön
    QueueHandle_t uart_queue;
    uart_event_t event;

    my_uart_config.source_clk = UART_SCLK_DEFAULT;
    my_uart_config.data_bits = UART_DATA_8_BITS;    // Data vi sänder är i 1 byte
    my_uart_config.baud_rate = 115200;
    my_uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    my_uart_config.parity = UART_PARITY_DISABLE;  // Sätter en check att vi har tagit emot data, denna ska vara samma
    my_uart_config.stop_bits = UART_STOP_BITS_1; // Sätter en stop bit när sänder data så vi vet när den slutar


    ESP_ERROR_CHECK_WITHOUT_ABORT(uart_param_config(uart_num, &my_uart_config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(uart_set_pin(uart_num, UART_PIN_TRANSMITT, UART_PIN_RECIEVE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK_WITHOUT_ABORT(uart_driver_install(uart_num, rx_buffer_size, rx_buffer_size, 10,&uart_queue, 0));


    TickType_t sentTimer = xTaskGetTickCount();

      // Read data from UART.
    uint8_t data[UART_RECIEVE_BUFFER_SIZE];      // data som ska recievas
    int length = 0;
   
    ESP_LOGI(TAG, "ms_to_tick %lu, tick_to_ms %lu", pdMS_TO_TICKS(30), pdTICKS_TO_MS(30));


    // Sen vi hade sentTimer blir våran elapsedTime
    while(1) {
        TickType_t elapsedtime = xTaskGetTickCount() - sentTimer;

        if(elapsedtime > pdMS_TO_TICKS(SEND_UART_PERIOD)) {
            sentTimer +=pdMS_TO_TICKS(SEND_UART_PERIOD);
            char* test_str = "Hello World!";
            int byteSent = uart_write_bytes(uart_num, (const char*)test_str, strlen(test_str));

            if(byteSent < 0) {
                ESP_LOGE(TAG, "ERROR writing uart");
            }
            else if(byteSent > 0) {
                ESP_LOGI(TAG, "Send %d bytes on uart", byteSent);
            }
            else {
                ESP_LOGI(TAG, "Sent no bytes on uart");
            }
        }
       
        // RECIEVE
        ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length)); 
        //ESP_LOGI(TAG, "Bytes in buffer %d", length);

        if(length > UART_RECIEVE_BUFFER_SIZE) {
            length = uart_read_bytes(uart_num, data, UART_RECIEVE_BUFFER_SIZE, pdMS_TO_TICKS(RECIEVE_WAIT_UART));
            
            if(length < 0) {
                ESP_LOGE(TAG, "ERROR recieving uart");
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
            else if(length > 0) {
                data[length] = 0; // sista byten gör den null terminated
                ESP_LOGI(TAG, "Recieve %d bytes on uart %s", length, data);
            }
            else {
                ESP_LOGI(TAG, "Recieve no bytes on uart");
                vTaskDelay(pdMS_TO_TICKS(30));
            }
        } else if(length > 0) {
            length = uart_read_bytes(uart_num, data, length, pdMS_TO_TICKS(RECIEVE_WAIT_UART));

            if(length < 0) {
                ESP_LOGE(TAG, "ERROR recieving uart");
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
            else if(length > 0) {
                data[length] = 0;
                ESP_LOGI(TAG, "Recieve %d bytes on uart %s", length, data);
            }
            else {
                ESP_LOGI(TAG, "Recieve no bytes on uart");
                vTaskDelay(pdMS_TO_TICKS(30));
            }
        }
        else {
            // Kommer här om finns ingeting att läsa
            //ESP_LOGI(TAG, "No msg got!\n");
            vTaskDelay(pdMS_TO_TICKS(30));
        }
    }
}