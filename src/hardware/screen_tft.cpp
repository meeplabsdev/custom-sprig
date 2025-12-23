#include "../hardware.h"
#include "screen_tft.h"

ScreenTFT::ScreenTFT()
{
    fill(0);
    init_screen_tft();
    set_backlight(true);
}

void ScreenTFT::draw()
{
    fill_start();

    for (int x = 0; x < 160; x++)
        for (int y = 0; y < 128; y++)
            fill_send(this->screen_buf[x][y]);

    fill_finish();
}

void ScreenTFT::fill(uint16_t colour)
{
    std::fill(
        &this->screen_buf[0][0],
        &this->screen_buf[0][0] + 160 * 128,
        colour);
}

void ScreenTFT::fill_callback(std::function<uint16_t(int, int)> callback, int x0, int y0, int x1, int y1)
{
    for (int x = x0; x < x1; x++)
        for (int y = y0; y < y1; y++)
            this->screen_buf[x][y] = callback(x, y);
}