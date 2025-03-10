# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)



set(EXTRA_COMPONENT_DIRS 
    "../../uppgift/uppgift_1/components/analog_led"
    "../../uppgift/uppgift_1/components/analog_to_digital"
    "../../uppgift/uppgift_1/components/binary_led"
    "../../uppgift/uppgift_1/components/button"
    "../../uppgift/uppgift_1/components/gpio"
    "../../uppgift/uppgift_1/components/led_controller"
    "../../uppgift/uppgift_1/components/nvs"
    "../../uppgift/uppgift_1/components/printer"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(projectLed)