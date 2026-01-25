#![allow(nonstandard_style)]
#![allow(dead_code)]

pub const HOLD_TIME_US: u32 = 1000000;

#[repr(C)]
pub enum BUTTON_TYPE {
    BOOTSEL,
    L_UP = 5,
    L_DOWN = 7,
    L_LEFT = 6,
    L_RIGHT = 8,
    R_UP = 12,
    R_DOWN = 14,
    R_LEFT = 13,
    R_RIGHT = 15,
}

#[repr(C)]
pub struct Button {
    _private: [u8; 0],
}

unsafe extern "C" {
    pub fn Button_new(pin: BUTTON_TYPE) -> *mut Button;
    pub fn Button_free(button: *mut Button);

    pub fn Button_is_pressed(button: *mut Button) -> bool;
    pub fn Button_is_long_pressed(button: *mut Button) -> bool;
    pub fn Button_was_pressed(button: *mut Button) -> bool;
    pub fn Button_was_long_pressed(button: *mut Button) -> bool;
}
