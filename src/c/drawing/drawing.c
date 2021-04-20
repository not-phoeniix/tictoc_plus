#include <pebble.h>
#include "../main.h"
#include "../messaging/msg.h"
extern Window *main_window;
extern ClaySettings settings;
#include <math.h>

static int hour, min, sec;

extern int *flag_colors[];
extern int num_stripes[];

void update_time() {
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);

  min = 360 * t->tm_min / 60;
  hour = 360 * (t->tm_hour % 12 * 6 + t->tm_min / 10) / (12 * 6);
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

static void draw_gay_hand(int length, int width, int rot, int segments, int colors[], GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  for (int i = 0; i < segments; i++) {
    
    GPoint point1 = {
      .x = center.x + (sin_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i) / TRIG_MAX_RATIO),
      .y = center.y - (cos_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i) / TRIG_MAX_RATIO),
    };

    GPoint point2 = {
      .x = center.x + (sin_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i + 1) / TRIG_MAX_RATIO),
      .y = center.y - (cos_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i + 1) / TRIG_MAX_RATIO)
    };

    graphics_context_set_stroke_color(ctx, GColorFromHEX(colors[i]));
    graphics_context_set_stroke_width(ctx, width);
    graphics_draw_line(ctx, point1, point2);
  }
}

static void draw_dot_bg(GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  graphics_context_set_fill_color(ctx, settings.hour_tick_color);
    
  int dist_from_center = 68;

  for(int i = 0; i < settings.num_of_dots; i++) {

    int angle = TRIG_MAX_ANGLE * i / settings.num_of_dots;

    GPoint dot = {
      .x = center.x + sin_lookup(angle) * dist_from_center / TRIG_MAX_RATIO,
      .y = center.y - cos_lookup(angle) * dist_from_center / TRIG_MAX_RATIO
    };

    graphics_fill_circle(ctx, dot, settings.hour_tick_size);
  }
}

static void draw_line_bg (GContext *ctx, int dist_from_center) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  graphics_context_set_stroke_color(ctx, settings.hour_tick_color);
  graphics_context_set_stroke_width(ctx, settings.hour_tick_size * 2);

  int second_dist = 100;

  for(int i = 0; i < settings.num_of_dots; i++) {

    int angle = TRIG_MAX_ANGLE * i / settings.num_of_dots;

    GPoint point1 = {
      .x = center.x + sin_lookup(angle) * dist_from_center / TRIG_MAX_RATIO,
      .y = center.y - cos_lookup(angle) * dist_from_center / TRIG_MAX_RATIO
    };

    GPoint point2 = {
      .x = center.x + sin_lookup(angle) * second_dist / TRIG_MAX_RATIO,
      .y = center.y - cos_lookup(angle) * second_dist / TRIG_MAX_RATIO
    };

    graphics_draw_line(ctx, point1, point2);
  }
}

void hands_draw_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  
  GColor gay_detect;

  if(settings.flag == 0) {
    gay_detect = settings.min_color;
  } else {
    gay_detect = settings.bg_color;
  }

  draw_hand(55, settings.hand_width, min, gay_detect, ctx);
  draw_hand(38, settings.hand_width, hour, settings.hour_color, ctx);

  graphics_context_set_fill_color(ctx, settings.dot_color);
  graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 4);
}

void draw_sec_update_proc(Layer *layer, GContext *ctx) {
  draw_hand(55, settings.second_width, sec, settings.sec_color, ctx);
}

void draw_hour_marks_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GPoint center = grect_center_point(&bounds);

  double x_scale = .9;
  double y_scale = .9;

  if (settings.dot_type == 1) {
    draw_line_bg(ctx, 70);
    graphics_context_set_fill_color(ctx, settings.bg_color);
    graphics_fill_rect(ctx, GRect(center.x - center.x * x_scale, center.y - center.y * y_scale, bounds.size.w * x_scale, bounds.size.h * y_scale), 0, GCornerNone);
  } else if (settings.dot_type == 2) {
    draw_dot_bg(ctx);
  } else {
    draw_line_bg(ctx, 70);
  }
}

void draw_gay_hand_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  
  draw_gay_hand(58, settings.hand_width, min, num_stripes[settings.flag], flag_colors[settings.flag], ctx);

  graphics_context_set_fill_color(ctx, settings.dot_color);
  graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 4);
}