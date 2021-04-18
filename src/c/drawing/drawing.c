#include <pebble.h>
#include "../main.h"
#include <math.h>
extern Window *main_window;
extern ClaySettings settings;

static int hour, min, sec;

void update_time() {
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);

  min = 360 * t->tm_min / 60;
  hour = 360 * (t->tm_hour % 12 + (1 - t->tm_min / 60)) / 12;
  sec = 360 * t->tm_sec / 60;
}

static void draw_hand(int length, int width, int rot, GColor color, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  GPoint end_point = {
    .x = center.x + (sin_lookup(DEG_TO_TRIGANGLE(rot)) * length / TRIG_MAX_RATIO),
    .y = center.y - (cos_lookup(DEG_TO_TRIGANGLE(rot)) * length / TRIG_MAX_RATIO)
  };

  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_stroke_width(ctx, width);
  graphics_draw_line(ctx, center, end_point);
}

void hands_draw_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  
  draw_hand(55, settings.hand_width, min, settings.min_color, ctx);
  draw_hand(35, settings.hand_width, hour, settings.hour_color, ctx);

  graphics_context_set_fill_color(ctx, settings.dot_color);
  graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 4);
}

void draw_sec_update_proc(Layer *layer, GContext *ctx) {
  draw_hand(55, settings.second_width, sec, settings.sec_color, ctx);
}

void draw_hour_marks_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  graphics_context_set_fill_color(ctx, settings.hour_tick_color);
    
  int dist_from_center = 65;

  for(int i = 0; i < 12; i++) {

    GPoint dot = {
      .x = center.x + sin_lookup(DEG_TO_TRIGANGLE(360 / 12 * i)) * dist_from_center,
      .y = center.y + cos_lookup(DEG_TO_TRIGANGLE(360 / 12 * i)) * dist_from_center
    };

    graphics_fill_circle(ctx, dot, settings.hand_width / 2);
  }
}