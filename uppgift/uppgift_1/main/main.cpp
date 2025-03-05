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
    char * data;
    myNvs::nvs nvs1;

    nvs1.init();

    data = nvs1.getDeviceName();
    ESP_LOGI("MAIN", "getDeviceName: %s", data);
    data = nvs1.getSerialNumber();
    ESP_LOGI("MAIN", "getSerialNumber: %s", data);

    //nvs1.setDeviceName("mattiasDevice");
    //nvs1.setSerialNumber("dd::cc::44");

    //nvs1.init();

  

}
