#include <pico/stdio.h>
#include <pico/cyw43_arch.h>
#include <pico/bootrom.h>
#include <hardware/clocks.h>

#include <algorithm>
#include <string>

#include "hardware.cpp"

#define FOREVER while (1)

static uint16_t RGB(uint8_t r, uint8_t b, uint8_t g)
{
    r = (uint8_t)((float)((float)r / 255.0f) * 31.0f);
    g = (uint8_t)((float)((float)g / 255.0f) * 31.0f);
    b = (uint8_t)((float)((float)b / 255.0f) * 63.0f);

    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

int l_power = 0;
int r_power = 0;
int delta = 1;

struct repeating_timer _main_task;
bool main_task(struct repeating_timer *rt)
{
    Hardware *hardware = (Hardware *)rt->user_data;

    hardware->status.set_brightness(l_power);
    hardware->screen.fill_callback(
        [](int x, int y)
        { return 0; }, 0, 0, 12, 6);
    hardware->screen.text(RGB(255, 255, 255), std::to_string(l_power), 0, 0);

    hardware->network.set_brightness(r_power);
    hardware->screen.fill_callback(
        [](int x, int y)
        { return 0; }, 148, 0, 160, 6);
    hardware->screen.text(RGB(255, 255, 255), std::to_string(r_power), 148, 0);

    if (hardware->leftpad.is_pressed(UP))
        l_power = std::clamp(l_power + delta, 0, 100);
    else if (hardware->leftpad.is_pressed(DOWN))
        l_power = std::clamp(l_power - delta, 0, 100);

    if (hardware->rightpad.is_pressed(UP))
        r_power = std::clamp(r_power + delta, 0, 100);
    else if (hardware->rightpad.is_pressed(DOWN))
        r_power = std::clamp(r_power - delta, 0, 100);

    return true;
}

struct repeating_timer _screen_task;
bool screen_task(struct repeating_timer *rt)
{
    Hardware *hardware = (Hardware *)rt->user_data;

    hardware->screen.draw();
    hardware->status.draw();
    hardware->network.draw();
    hardware->builtin.draw();

    return true;
}

struct repeating_timer _bootsel_task;
bool bootsel_task(struct repeating_timer *rt)
{
    Hardware *hardware = (Hardware *)rt->user_data;

    if (hardware->bootsel.is_pressed())
    {
        reset_usb_boot(0, 0);
        return false;
    }

    return true;
}

int main()
{
    if (!stdio_init_all() ||
        !set_sys_clock_khz(270000, true) ||
        cyw43_arch_init_with_country(21067))
    {
        printf("Init failed");
        return -1;
    }

    Hardware hardware = Hardware();

    add_repeating_timer_ms(10, main_task, &hardware, &_main_task);
    add_repeating_timer_ms(50, screen_task, &hardware, &_screen_task);
    add_repeating_timer_ms(100, bootsel_task, &hardware, &_bootsel_task);

    hardware.screen.fill_callback(
        [](int x, int y)
        { return RGB(0, x * 255 / 160, y * 255 / 128); },
        0, 0, 160, 128);
    hardware.screen.text(RGB(255, 255, 255), "Hello world!", 0, 122);

    FOREVER tight_loop_contents();
}