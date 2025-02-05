idf_component_register(SRCS "myTask.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver freertos
                    REQUIRES esp_driver_gpio)
