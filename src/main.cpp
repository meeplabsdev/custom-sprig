#include <pico/stdio.h>
#include <pico/cyw43_arch.h>
#include <pico/bootrom.h>

#include <algorithm>

#include "hardware.h"

#define FOREVER while (1)

struct repeating_timer _bootsel_task;
bool bootsel_task(struct repeating_timer *rt)
{
    if (get_button(BUTTON::BOOTSEL))
    {
        reset_usb_boot(0, 0);
        return false;
    }

    return true;
}

float l_power = 0.0;
float r_power = 0.0;
float delta = 0.1;

struct repeating_timer _main_task;
bool main_task(struct repeating_timer *rt)
{
    set_led(LED::STATUS, l_power);
    set_led(LED::NETWORK, r_power);

    if (get_button(BUTTON::W))
        l_power = std::clamp(l_power + delta, 0.0f, 1.0f);
    else if (get_button(BUTTON::S))
        l_power = std::clamp(l_power - delta, 0.0f, 1.0f);

    if (get_button(BUTTON::I))
        r_power = std::clamp(r_power + delta, 0.0f, 1.0f);
    else if (get_button(BUTTON::K))
        r_power = std::clamp(r_power - delta, 0.0f, 1.0f);

    return true;
}

int main()
{
    if (!stdio_init_all() ||
        !hardware_init_all())
    {
        printf("Init failed");
        return -1;
    }

    add_repeating_timer_ms(100, bootsel_task, NULL, &_bootsel_task);
    add_repeating_timer_ms(10, main_task, NULL, &_main_task);

    FOREVER tight_loop_contents();
}