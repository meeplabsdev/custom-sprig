#include "hardware/button.h"
#include "hardware/led.h"
#include "hardware/screen_tft.h"
#include <functional>

#ifndef __HARDWARE__
#define __HARDWARE__

enum LED
{
    STATUS,
    NETWORK,
    PICO,
};

enum BUTTON
{
    W,
    S,
    A,
    D,
    I,
    K,
    J,
    L,
    BOOTSEL,
};

bool initialized = false;
bool hardware_init_all()
{
    init_button(BUTTON_W);
    init_button(BUTTON_A);
    init_button(BUTTON_S);
    init_button(BUTTON_D);
    init_button(BUTTON_I);
    init_button(BUTTON_J);
    init_button(BUTTON_K);
    init_button(BUTTON_L);

    init_led_sided(LED_L);
    init_led_sided(LED_R);

    bool success =
        init_led_builtin();

    init_screen_tft();

    initialized = true;
    return success;
}

bool get_button(enum BUTTON button)
{
    if (button == W)
        return get_button(BUTTON_W);
    else if (button == S)
        return get_button(BUTTON_S);
    else if (button == A)
        return get_button(BUTTON_A);
    else if (button == D)
        return get_button(BUTTON_D);
    else if (button == I)
        return get_button(BUTTON_I);
    else if (button == J)
        return get_button(BUTTON_J);
    else if (button == K)
        return get_button(BUTTON_K);
    else if (button == L)
        return get_button(BUTTON_L);
    else
        return get_bootsel_button();
}

void set_led(enum LED led, float power)
{
    if (led == STATUS)
        set_led_sided(LED_L, power);
    else if (led == NETWORK)
        set_led_sided(LED_R, power);
    else
        set_led_builtin(power);
}

void draw_pixel(uint16_t colour)
{
    fill_start();
    fill_send(colour);
    fill_finish();
}

void draw_callback(std::function<uint8_t(int, int)> callback, int x0, int y0, int x1, int y1)
{
    fill_start();
    // left to right, top to bottom
    for (int x = x0; x < x1; x++)
    {
        for (int y = y0; y < y1; y++)
        {
            fill_send(callback(x, y));
        }
    }
    fill_finish();
}

void draw_rectangle(uint16_t colour, int x0, int y0, int x1, int y1)
{
    draw_callback(
        [colour](int x, int y)
        { return colour; },
        x0, y0, x1, y1);
}

void draw_screen(uint16_t colour)
{
    draw_rectangle(colour, 0, 0, 160, 128);
}

#endif