#include <pebble.h>
#include "main.h"
#include "drawing/drawing.h"
#include "config/cfg.h"

Window *main_window;
static Layer *clock_hands, *bg_layer;

ClaySettings settings;

static double two_pi = 2 * 3.1415926535;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  layer_mark_dirty(clock_hands);
}

void update_stuff() {
  update_time();
  layer_mark_dirty(clock_hands);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  window_set_background_color(main_window, GColorBlack);
  
  update_time();

  clock_hands = layer_create(bounds);
  layer_set_update_proc(clock_hands, hands_draw_update_proc);
  layer_add_child(window_layer, clock_hands);

  bg_layer = layer_create(bounds);
  layer_set_update_proc(bg_layer, draw_hour_marks_update_proc);
  layer_add_child(window_layer, bg_layer);
}

static void main_window_unload() {
  layer_destroy(clock_hands);
}

static void init() {
  main_window = window_create();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  load_settings();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(main_window, true);
  update_stuff();
}

static void deinit() {
  window_destroy(main_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
