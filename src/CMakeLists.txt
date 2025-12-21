if(PICO_BOARD STREQUAL "pico_w")
  add_executable(firmware main.cpp)
  target_link_libraries(
    firmware pico_stdlib 
    pico_cyw43_arch_none
    hardware_pwm
    hardware_spi
  )
endif()

pico_add_extra_outputs(firmware)
example_auto_set_url(firmware)
