
#include "bit_bang.h"

namespace myBitBang {
    bitBang::bitBang() {

    }

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