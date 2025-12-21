#include <pico/stdio.h>
#include <pico/cyw43_arch.h>
#include <pico/bootrom.h>

#include "hardware.h"

#define FOREVER while (1)

struct repeating_timer _bootsel_task;
bool bootsel_task(struct repeating_timer *rt)
{
    if (get_button(BOOTSEL))
    {
        reset_usb_boot(0, 0);
        return false;
    }

    return true;
}

int main()
{
    // init
    stdio_init_all();
    if (!hardware_init_all())
    {
        printf("Init failed");
        return -1;
    }

    // tasks
    add_repeating_timer_ms(20, bootsel_task, NULL, &_bootsel_task);

    // main
    set_led(STATUS, 1);

    float power = 0.0;
    float delta = 0.1;

    FOREVER
    {
        sleep_ms(10);
        set_led(NETWORK, power);

        power += delta;
        if (power >= 1.0 || power <= 0.0)
        {
            delta = -delta;
            sleep_ms(500);
        }
    }
}