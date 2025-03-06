#include "esp_mac.h"
#include "nvs.h"
//#include "esp_log.h"
#include "analog_to_digital.h"
#include "analog_led.h"
#include "binary_led.h"
#include "led_controller.h"
#include "bit_bang.h"
#include "GPIO.h"


extern "C" void app_main(void)
{  
    myNvs::nvs nvs1;

    nvs1.init();

    char* data1;
    
    if(nvs1.getDeviceName(&data1) == ESP_OK) {
        ESP_LOGI("MAIN", "getDeviceName: %s", data1);
    };
    
    // nvs1.setDeviceName("hejsan");
    // nvs1.setDeviceName("hejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhhhejhhhhhhhhhhhhhhhhhhhh");
    //nvs1.setDeviceName("TJABA");

    //nvs1.setSerialNumber("aa::bb::cc");
    //nvs1.setSerialNumber("dd::ee::ffffffff");


    //data = nvs1.getSerialNumber();
    //ESP_LOGI("MAIN", "getSerialNumber: %s", data);

    //nvs1.setDeviceName("hejsan");

    //nvs1.setDeviceName("tja");

    //nvs1.setDeviceName("dennisDevice");
    //nvs1.setSerialNumber("dd::cc::33");

    //nvs1.init();

  

}
