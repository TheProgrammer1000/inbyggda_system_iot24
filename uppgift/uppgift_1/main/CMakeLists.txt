idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES button analog_to_digital binary_led analog_led nvs led_controller bit_bang gpio bluetoothLE)