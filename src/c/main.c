#include <pebble.h>
#include "main.h"
#include <math.h>

static Window *main_window;
static Layer *clock_hands;
static int hour, min;

static double two_pi = 2 * 3.1415926535;

static int hand_width = 7;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char h_temp[3];
  static char m_temp[3];

  strftime(h_temp, sizeof(h_temp), "%I", tick_time);
  strftime(m_temp, sizeof(m_temp), "%M", tick_time);

  hour = atoi(h_temp);
  min = atoi(m_temp);
}

static void draw_hand(int length, int width, int rot, GColor color, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));

  GPoint center = {
    .x = bounds.size.w / 2,
    .y = bounds.size.h / 2
  };

  GPoint end_point = {
    .x = center.x + (sin(rot) * length),
    .y = center.y - (cos(rot) * length)
  };

  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_stroke_width(ctx, width);
  graphics_draw_line(ctx, center, end_point);
}

static void hands_draw_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));

  int m_degree = min / 60 * two_pi;
  int h_degree = (hour + min / 60) / 12 * two_pi;

  draw_hand(55, hand_width, m_degree, GColorWhite, ctx);
  draw_hand(35, hand_width, h_degree, GColorRed, ctx);

  graphics_context_set_fill_color(ctx, GColorRed);
  graphics_fill_circle(ctx, GPoint(bounds.size.w, bounds.size.h), 3);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  layer_mark_dirty(clock_hands);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  window_set_background_color(main_window, GColorBlack);
  
  clock_hands = layer_create(bounds);
  layer_set_update_proc(clock_hands, hands_draw_update_proc);
  layer_add_child(window_layer, clock_hands);

  update_time();
}

static void main_window_unload() {
  layer_destroy(clock_hands);
}

static void init() {
  main_window = window_create();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(main_window, true);
  update_time();
  layer_mark_dirty(clock_hands);
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
