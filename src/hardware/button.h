#include <hardware/sync.h>
#include <hardware/regs/io_qspi.h>
#include <hardware/structs/ioqspi.h>

#define BUTTON_W 5
#define BUTTON_A 6
#define BUTTON_S 7
#define BUTTON_D 8
#define BUTTON_I 12
#define BUTTON_J 13
#define BUTTON_K 14
#define BUTTON_L 15

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

// W, S, A, D, I, K, J, L
// uint button_pins[] = {5, 7, 6, 8, 12, 14, 13, 15};

void init_button(uint button)
{
    gpio_set_dir(button, GPIO_IN);
    gpio_pull_up(button);
}

bool get_button(uint button)
{
    return !gpio_get(button);
}