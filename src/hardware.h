#include "hardware/button.h"
#include "hardware/led.h"
#include "hardware/screen.h"

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
    init_led_sided(LED_L);
    init_led_sided(LED_R);

    init_button(BUTTON_W);
    init_button(BUTTON_A);
    init_button(BUTTON_S);
    init_button(BUTTON_D);
    init_button(BUTTON_I);
    init_button(BUTTON_J);
    init_button(BUTTON_K);
    init_button(BUTTON_L);

    bool success =
        init_led_builtin();

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

#endif