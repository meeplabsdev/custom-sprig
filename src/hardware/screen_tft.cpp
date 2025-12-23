#include "../hardware.h"
#include "screen_tft.h"

#include "../font.cpp"
#include <string>

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

int char_index(char character)
{
    size_t index = 0;
    while (chars[index] != '\0')
    {
        if (chars[index] == character)
            return index;
        ++index;
    }

    return -1;
}

void ScreenTFT::character(uint16_t colour, char character, int x, int y)
{
    int index = char_index(character);
    if (index == -1)
        return;

    auto c = font[index];
    for (int row = 0; row < 6; row++)
    {
        auto row_data = c[row];
        for (int i = 0; i < 8; i++)
            if ((row_data >> i) & 1 == 1)
            {
                int x0 = x + (4 - i);
                int y0 = y + row;

                if (x0 < 0 || x0 > 160 || y0 < 0 || y0 > 128)
                    continue;

                this->screen_buf[x0][y0] = colour;
            }
    }
}

void ScreenTFT::text(uint16_t colour, std::string contents, int x, int y)
{
    int i = 0;
    for (char c : contents)
    {
        character(colour, c, x + i, y);
        i += 4;
    }
}