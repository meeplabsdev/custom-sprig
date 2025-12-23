#include "../hardware.h"
#include <hardware/pwm.h>

LED::LED() {}
LED::LED(LED_TYPE type, bool pwm)
{
    this->type = type;
    this->pwm = pwm;
    this->value = 0.0;

    if (pwm)
    {
        uint slice_num = pwm_gpio_to_slice_num(type);

        gpio_set_function(type, GPIO_FUNC_PWM);
        pwm_set_enabled(slice_num, true);
    }
    else if (type != PICO)
    {
        gpio_set_function(type, GPIO_FUNC_SIO);
        gpio_set_dir(type, GPIO_OUT);
    }
}

void LED::set_powered(bool powered)
{
    if (this->type == PICO)
        return cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, powered);

    if (!this->pwm)
        return gpio_put(this->type, powered);

    uint slice_num = pwm_gpio_to_slice_num(this->type);
    uint channel = pwm_gpio_to_channel(this->type);
    uint16_t level = powered ? 65535 / 8 : 0;

    pwm_set_chan_level(slice_num, channel, level);
}

void LED::set_brightness(float brightness)
{
    if (brightness > 1 || brightness < 0)
        return;

    if (!this->pwm)
        return set_powered(brightness > 0.5);

    uint slice_num = pwm_gpio_to_slice_num(this->type);
    uint channel = pwm_gpio_to_channel(this->type);
    uint16_t level = 65535 / 8 * brightness;

    pwm_set_chan_level(slice_num, channel, level);
}