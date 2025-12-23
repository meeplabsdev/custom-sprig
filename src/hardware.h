#include <functional>
#include <cstdint>

#ifndef __HARDWARE__
#define __HARDWARE__

enum LED_TYPE
{
    PICO,
    STATUS = 28,
    NETWORK = 4,
};

class LED
{
private:
    LED_TYPE type;
    bool pwm;
    float value;

public:
    LED();
    LED(LED_TYPE type, bool pwm);
    void set_powered(bool powered);
    void set_brightness(float brightness);
};

enum BUTTON_TYPE
{
    BOOTSEL,
    L_UP = 5,
    L_DOWN = 7,
    L_LEFT = 6,
    L_RIGHT = 8,
    R_UP = 12,
    R_DOWN = 14,
    R_LEFT = 13,
    R_RIGHT = 15,
};

class Button
{
private:
    BUTTON_TYPE type;
    bool pressed;

public:
    Button();
    Button(BUTTON_TYPE type);
    bool is_pressed();
};

enum PAD_DIRECTION
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

class ButtonPad
{
private:
    Button buttons[4];

public:
    ButtonPad();
    ButtonPad(Button up, Button down, Button left, Button right);
    bool is_pressed(PAD_DIRECTION direction);
};

class ScreenTFT
{
private:
    uint16_t screen_buf[160][128]; // [x][y]

public:
    ScreenTFT();
    void draw();
    void fill(uint16_t colour);
    void fill_callback(std::function<uint16_t(int, int)> callback, int x0, int y0, int x1, int y1);
};

class Hardware
{
private:
    bool initialized = false;

public:
    Hardware();
    LED status;
    LED network;
    LED builtin;
    Button bootsel;
    ButtonPad leftpad;
    ButtonPad rightpad;
    ScreenTFT screen;
};

#endif
