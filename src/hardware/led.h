#include <hardware/pwm.h>

#define LED_L 28
#define LED_R 4

void set_led_sided(uint led, float power)
{
    if (power > 1 || power < 0)
        return;

    uint slice_num = pwm_gpio_to_slice_num(led);
    uint channel = pwm_gpio_to_channel(led);
    uint16_t level = 65535 / 8 * power;

    pwm_set_chan_level(slice_num, channel, level);
}

void init_led_sided(uint led)
{
    uint slice_num = pwm_gpio_to_slice_num(led);

    gpio_set_function(led, GPIO_FUNC_PWM);
    pwm_set_enabled(slice_num, true);
    set_led_sided(led, 0);
}

bool set_led_builtin(float power)
{
    if (power > 1 || power < 0)
        return false;

    bool enabled = power > 0.5;
    if (cyw43_gpio_set(&cyw43_state, CYW43_WL_GPIO_LED_PIN, enabled))
    {
        return false;
    }

    return true;
}

bool init_led_builtin()
{
    if (cyw43_arch_init_with_country(21067))
    {
        return false;
    }

    set_led_builtin(0);
    return true;
}