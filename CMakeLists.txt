cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_micro_ros_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(external/pico-distance-sensor/src)

link_directories(
    ${CMAKE_SOURCE_DIR}/external/libmicroros
)

add_executable(pico_rmrc_uros
    src/main.cpp
    src/pico_ros_basic.c
    src/pico_uart_transport.c
)

target_link_libraries(pico_rmrc_uros
    pico_stdlib
    hardware_pio 
    hardware_irq
    distance-sensor 
    microros
)

target_include_directories(pico_rmrc_uros PUBLIC
    external/libmicroros/include
    external/pico-distance-sensor/include
    include/
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico
pico_enable_stdio_usb(pico_rmrc_uros 1)
pico_enable_stdio_uart(pico_rmrc_uros 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2
pico_add_extra_outputs(pico_rmrc_uros)
