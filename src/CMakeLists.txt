add_executable(firmware main.cpp)

add_subdirectory(fatfs/src build)

target_link_libraries(
  firmware
  pico_stdlib 
  pico_cyw43_arch_none
  pico_audio_i2s
  hardware_adc
  hardware_pwm
  hardware_spi
  hardware_timer
  no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
)

target_compile_definitions(firmware PRIVATE
        PICO_AUDIO_I2S_MONO_INPUT=1
        USE_AUDIO_I2S=1
        PICO_AUDIO_I2S_DATA_PIN=9
        PICO_AUDIO_I2S_CLOCK_PIN_BASE=10
)

# pico_add_extra_outputs(firmware)
example_auto_set_url(firmware)
