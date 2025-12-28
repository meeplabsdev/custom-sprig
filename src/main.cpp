#include <pico/stdio.h>
#include <pico/cyw43_arch.h>
#include <pico/bootrom.h>
#include <hardware/clocks.h>

#include <algorithm>
#include <string>

#include "hardware.cpp"
#include "ff.h"

#define FOREVER while (1)

static uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
    r = (uint8_t)((float)((float)r / 255.0f) * 31.0f);
    b = (uint8_t)((float)((float)b / 255.0f) * 31.0f);
    g = (uint8_t)((float)((float)g / 255.0f) * 63.0f);

    return ((r & 0b11111000) << 8) | ((b & 0b11111100) << 3) | (g >> 3);
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

struct repeating_timer _main_task;
bool main_task(struct repeating_timer *rt)
{
    Hardware *hardware = (Hardware *)rt->user_data;

    return true;
}

int main()
{
    timer_hw->dbgpause = 0;
    if (!stdio_init_all() ||
        !set_sys_clock_khz(270000, true) ||
        cyw43_arch_init_with_country(21067))
    {
        printf("Init failed");
        return -1;
    }

    Hardware hardware = Hardware();
    hardware.screen.fill(RGB(0, 0, 0));
    hardware.screen.draw();
    hardware.screen.set_backlight(true);

    FRESULT fr_mount = hardware.storage.mount();
    if (!hardware.storage.is_ready())
    {
        hardware.screen.text(RGB(255, 255, 255), "SD card not detected.", 6, 6);
        hardware.screen.text(RGB(200, 200, 200), "Insert an SD card and reboot.", 6, 12);
        hardware.screen.text(RGB(255, 100, 100), "MOUNT " + std::to_string(fr_mount), 6, 24);
        hardware.screen.text(RGB(200, 200, 200), "NO_SD", 6, 110);
        hardware.screen.draw();

        return -1;
    }

    sine(0x800000, 16);
    hardware.screen.fill(RGB(0, 0, 0));

    // add_repeating_timer_ms(100, bootsel_task, &hardware, &_bootsel_task);
    add_repeating_timer_ms(50, screen_task, &hardware, &_screen_task);
    // add_repeating_timer_ms(10, main_task, &hardware, &_main_task);

    uint16_t buffer[160 * 128];
    hardware.storage.read("logo.bin", buffer, 0);
    hardware.screen.fill_callback(
        [buffer](int x, int y)
        { return buffer[y * 160 + x]; },
        0, 0, 160, 128);

    FOREVER tight_loop_contents();
}