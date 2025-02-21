#include "nvs.h"
#include "esp_log.h"


void myFunctionToAnalog(char* msg) {
    //printf("%s" NEW_LINE, msg);
    printf("%s\n", msg);
}

void myFunctionToPotenti() {
    printf("From potentimeter!\n");
}


extern "C" void app_main(void)
{  
    myNvs::nvs nvs;
    
    nvs.init();
    //ESP_ERROR_CHECK(nvs_erase_all(nvs.getNvsHandle()));
    //nvs.init();
    // printf("my serialValue: %s\n",nvs.getSerialNumber());
    // printf("my deviceNAme: %s\n",nvs.getDeviceName());

    //nvs.setDeviceName("myDev2");
    //nvs.setSerialNumber("AA:BB:CC:DD22");


    //printf("\nHello from main!\n");
}