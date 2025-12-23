#include "../hardware.h"

ButtonPad::ButtonPad() {}
ButtonPad::ButtonPad(Button up, Button down, Button left, Button right)
{
    this->buttons[0] = up;
    this->buttons[1] = down;
    this->buttons[2] = left;
    this->buttons[3] = right;
}

bool ButtonPad::is_pressed(PAD_DIRECTION direction)
{
    return this->buttons[direction].is_pressed();
}