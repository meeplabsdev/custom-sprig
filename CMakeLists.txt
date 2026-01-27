cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(firmware C CXX ASM)

pico_sdk_init()

add_subdirectory(opensprig build)

add_library(rust STATIC IMPORTED GLOBAL)
set_target_properties(rust PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/firmware.a)

add_executable(firmware)

target_link_libraries(firmware
    rust
    opensprig
    pico_stdlib
    pico_cyw43_arch_none
)

target_link_options(firmware PRIVATE
    "-Wl,--print-memory-usage"
)
if("$ENV{OPENSPRIG_APPLICATION}" STREQUAL "1")
  pico_set_linker_script(firmware ${CMAKE_CURRENT_LIST_DIR}/memmap.ld)
endif()

pico_enable_stdio_uart(firmware 1)
pico_enable_stdio_usb(firmware 1)
pico_add_extra_outputs(firmware)