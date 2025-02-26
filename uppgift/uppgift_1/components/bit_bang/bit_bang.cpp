
#include "bit_bang.h"

namespace myBitBang {
    bitBang::bitBang() {

    }

    /**
     * Skicka data en 1 som indikerar att nu komemr ett meddelande
     * efter denna bit som är 1 kommer meddlande på 4 bitar!
     *  
     * Ha en timer en takt som går en gång i sekunden och lyssnar på 
     * meddalande som kommer in
     * 
     */

    void bitBang::init() {

        int bundleA_gpios[2] = {2, 3};

        uint32_t bundlePointer = (uint32_t)bundleA_gpios;
        // Create bundleA, output only
        dedic_gpio_bundle_handle_t bundleA = NULL;
          
        
        dedic_gpio_bundle_config_t bundleA_config = {
            .gpio_array = bundleA_gpios,
            .array_size = sizeof(bundleA_gpios) / sizeof(bundleA_gpios[0]),
            .flags {
                .out_en = 1,
            }
        };
        ESP_ERROR_CHECK(dedic_gpio_new_bundle(&bundleA_config, &bundleA));

        dedic_gpio_get_out_mask(bundleA, &bundlePointer);
    }
}