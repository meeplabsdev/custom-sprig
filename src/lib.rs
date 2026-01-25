#![no_std]
#![no_main]
#![cfg(not(test))]

mod bridge;
use crate::bridge::button_bridge::*;
use crate::bridge::led_bridge::*;

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn main() -> ! {
    unsafe {
        let led: *mut LED = LED_new(LED_TYPE::STATUS, true);
        let button: *mut Button = Button_new(BUTTON_TYPE::L_LEFT);

        loop {
            let pressed = Button_is_pressed(button);
            LED_set_brightness_bool(led, pressed);
        }
    }
}
