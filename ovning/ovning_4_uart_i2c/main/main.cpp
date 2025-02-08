#include <stdio.h>
#include "driver/uart.h"
#include <string.h>
#include "esp_check.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "printer.h"

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

    PRINTF_COLOR(ANSI_MAGENTA, "sentTimer: %ld" NEW_LINE, sentTimer);

      // Read data from UART.
    uint8_t data[UART_RECIEVE_BUFFER_SIZE + 1];       // data som ska recievas
    int length = 0;
   
    ESP_LOGI(TAG, "ms_to_tick %lu, tick_to_ms %lu", pdMS_TO_TICKS(30), pdTICKS_TO_MS(30));


    TickType_t startTimer = xTaskGetTickCount();

    uart_event_t recievedEvent;


    // Sen vi hade sentTimer blir våran elapsedTime
    while(1) {
        
            TickType_t elapsedTimeSinceStart = xTaskGetTickCount() - startTimer;



            if(elapsedTimeSinceStart > pdMS_TO_TICKS(SEND_UART_PERIOD)) {

                startTimer += pdMS_TO_TICKS(SEND_UART_PERIOD);
                char* test_str = "Hello World!";
                int byteSent = uart_write_bytes(uart_num, (const char*)test_str, strlen(test_str));

                if(byteSent < 0) {
                    ESP_LOGE(TAG, "ERROR writing uart");
                    vTaskDelay(pdMS_TO_TICKS(2000));
                }
                else if(byteSent > 0) {
                    ESP_LOGI(TAG, "Send %d bytes on uart", byteSent);
                    vTaskDelay(pdMS_TO_TICKS(30));
                }
                else {
                    ESP_LOGI(TAG, "Sent no bytes on uart");
                    vTaskDelay(pdMS_TO_TICKS(30));
                }
            }


            
            //PRINTF_COLOR(ANSI_MAGENTA, "elapsedTimeSinceStart: %ld" NEW_LINE, elapsedTimeSinceStart);
            
        /*
                UART_BREAK,             /*!< UART break event*/
        //UART_BUFFER_FULL,       /*!< UART RX buffer full event*/
        //UART_FIFO_OVF,          /*!< UART FIFO overflow event*/
        //UART_FRAME_ERR,         /*!< UART RX frame error event*/
        //UART_PARITY_ERR,        /*!< UART RX parity event*/
        //UART_DATA_BREAK,        /*!< UART TX data and break event*/
        //UART_PATTERN_DET,       /*!< UART pattern detected */
        
        

        if(xQueueReceive(uart_queue, &recievedEvent, pdMS_TO_TICKS(30)) == pdTRUE) {
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));

            int bytesFromBuffer = uart_read_bytes(uart_num, &data, length, pdMS_TO_TICKS(30));

            if(bytesFromBuffer < 0) {
                ESP_LOGE(TAG, "Failed to read from buffer");
            }
            else if(bytesFromBuffer > UART_RECIEVE_BUFFER_SIZE)  {
                bytesFromBuffer = uart_read_bytes(uart_num, &data, UART_RECIEVE_BUFFER_SIZE, pdMS_TO_TICKS(30));
                if(bytesFromBuffer < 0) {
                    ESP_LOGE(TAG, "Error reading from Buffer");
                }
                else if(bytesFromBuffer > 0) {
                    switch (recievedEvent.type)
                    {
                    case UART_DATA:
                            PRINTF_COLOR(ANSI_MAGENTA, "Receieved %d bytes and text: %s", bytesFromBuffer, (char*) data);
                        break;
                    
                    default:
                        break;
                    }

                }
            }
            else {
                switch (recievedEvent.type)
                {
                case UART_DATA:
                        PRINTF_COLOR(ANSI_MAGENTA, "Receieved %d bytes and text: %s", bytesFromBuffer, (char*) data);
                    break;
                
                default:
                    break;
                }
            }
        }  
        else {
            PRINTF_GROUP_FAILED("Something went wrong with xQueueRecieve!" NEW_LINE);
        }
    }
}