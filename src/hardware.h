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
    BOOTSEL,
};

bool initialized = false;
bool hardware_init_all()
{
    init_led_sided(LED_L);
    init_led_sided(LED_R);

    bool success =
        init_led_builtin();

    initialized = true;
    return success;
}

bool get_button(enum BUTTON button)
{
    if (button == BOOTSEL)
    {
        return get_bootsel_button();
    }

    return false;
}

void set_led(enum LED led, float power)
{
    if (led == STATUS)
    {
        set_led_sided(LED_L, power);
    }
    else if (led == NETWORK)
    {
        set_led_sided(LED_R, power);
    }
    else
    {
        set_led_builtin(power);
    }
}

#endif