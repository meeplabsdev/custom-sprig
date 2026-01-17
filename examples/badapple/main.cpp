#include <pico/cyw43_arch.h>
#include <pico/stdio.h>
#include <screen.h>
#include <speaker.h>
#include <storage.h>

#define FOREVER while (1)

struct task_args {
  Screen *screen;
  Speaker *speaker;
  Storage *storage;
  FIL *file;
};

struct repeating_timer _screen_task;
bool screen_task(repeating_timer_t *rt) {
  task_args *args = (task_args *)rt->user_data;
  args->screen->blit();
  return true;
}

struct repeating_timer _main_task;
bool main_task(repeating_timer_t *rt) {
  // read straigt into the screen buffer
  task_args *args = (task_args *)rt->user_data;
  FRESULT fr = args->storage->raw_read(args->file, args->screen->screen_buf,
                                       FRAME_SIZE * 2);

  if (fr != FR_OK) {
    args->screen->draw_text(RGB(255, 100, 100), std::to_string(fr), 6, 6);
    f_close(args->file);
    return false;
  }

  return true;
}

int main() {
  Screen screen = Screen();
  Speaker speaker = Speaker();
  Storage storage = Storage();

  FRESULT fr_mount = storage.mount();
  if (!storage.is_ready()) {
    screen.draw_text(RGB(200, 200, 200), "NO_SD", 6, 110);
    screen.blit();

    return -1;
  }

  FIL file;
  f_open(&file, "badapple.bin", FA_READ);

  task_args args = {
      &screen,
      &speaker,
      &storage,
      &file,
  };

  add_repeating_timer_ms(-82, screen_task, &args, &_screen_task);

  storage.read("logo.bin", screen.screen_buf, FRAME_SIZE * 2, 0);
  screen.set_backlight(true);
  speaker.sine(0x800000, 16);

  sleep_ms(800);

  add_repeating_timer_ms(-50, main_task, &args, &_main_task);

  FOREVER tight_loop_contents();
}
