#include "hardware/button_pad.cpp"
#include "hardware/button.cpp"
#include "hardware/led.cpp"
#include "hardware/screen_tft.cpp"
#include "hardware.h"

Hardware::Hardware()
{
    if (this->initialized)
        return;

    // LEDs
    this->status = LED(STATUS, true);
    this->network = LED(NETWORK, true);
    this->builtin = LED(PICO, false);

    // Buttons
    this->bootsel = Button(BOOTSEL);

    // ButtonPads
    this->leftpad = ButtonPad(
        Button(L_UP),
        Button(L_DOWN),
        Button(L_LEFT),
        Button(L_RIGHT));
    this->rightpad = ButtonPad(
        Button(R_UP),
        Button(R_DOWN),
        Button(R_LEFT),
        Button(R_RIGHT));

    // Screen
    this->screen = ScreenTFT();

    this->initialized = true;
}
