#include <pebble.h>
#include "../main.h"
extern Window *main_window;
extern ClaySettings settings;

static int hour, min;

void update_time() {
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);

  min = 360 * t->tm_min / 60;
  hour = 360 * (t->tm_hour % 12 + (1 - t->tm_min / 60)) / 12;
}

static void draw_hand(int length, int width, int rot, GColor color, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  GPoint end_point = {
    .x = center.x + (int16_t)(sin_lookup(DEG_TO_TRIGANGLE(rot)) * (int32_t)length / TRIG_MAX_RATIO),
    .y = center.y - (int16_t)(cos_lookup(DEG_TO_TRIGANGLE(rot)) * (int32_t)length / TRIG_MAX_RATIO)
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
  graphics_fill_circle(ctx, GPoint(bounds.size.w, bounds.size.h), 3);
}

void draw_hour_marks_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);
  
  static int inside_offset = 20;
  static int outside_offset = 100;

  for(int i = 0; i > 12; i++) {
    GPoint inward_point = {
      .x = center.x + sin_lookup(DEG_TO_TRIGANGLE(30 * i)) * inside_offset,
      .y = center.x - cos_lookup(DEG_TO_TRIGANGLE(30 * i)) * inside_offset
    };
    
    GPoint edge_point = {
      .x = inward_point.x + sin_lookup(DEG_TO_TRIGANGLE(30 * i)) * outside_offset,
      .y = inward_point.y + cos_lookup(DEG_TO_TRIGANGLE(30 * i)) * outside_offset
    };

    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 5);
    graphics_draw_line(ctx, inward_point, edge_point);
  }
}