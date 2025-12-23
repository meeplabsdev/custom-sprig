#include "../hardware.h"
#include <hardware/sync.h>
#include <hardware/structs/ioqspi.h>

// https://github.com/raspberrypi/pico-examples/blob/master/picoboard/button/button.c
bool __no_inline_not_in_flash_func(get_bootsel_button)()
{
    bool state = false;
    uint32_t flags = save_and_disable_interrupts();

    hw_write_masked(&ioqspi_hw->io[1].ctrl,
                    GPIO_OVERRIDE_LOW << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);

    for (volatile int i = 0; i < 1000; ++i)
        ;
    state = !(sio_hw->gpio_hi_in & (1u << 1));

    hw_write_masked(&ioqspi_hw->io[1].ctrl,
                    GPIO_OVERRIDE_NORMAL << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);

    restore_interrupts(flags);
    return state;
}

Button::Button() {}
Button::Button(BUTTON_TYPE type)
{
    this->type = type;
    this->pressed = false;

    if (type == BOOTSEL)
        return;

    gpio_set_function(type, GPIO_FUNC_SIO);
    gpio_set_dir(type, GPIO_IN);
    gpio_pull_up(type);
}

bool Button::is_pressed()
{
    if (this->type == BOOTSEL)
        return get_bootsel_button();

    return !gpio_get(this->type);
}