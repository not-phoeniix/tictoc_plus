#include <pebble.h>
#include "../main.h"
#include "../messaging/msg.h"
#include <math.h>

int hour, min, sec;     // hour, min, sec as degrees
int actual_hour;        // hour as an integer
static char date_char[] = "DD";

extern int *flag_colors[];
extern int num_stripes[];

/// @brief Returns one of the two integers 
//         depending on visibility of quickview panel.
/// @param if_yes Integer to return if quickview is shown
/// @param if_no Integer to return if quickview is not shown
/// @return Either if_yes or if_no
int if_quickview_else(int if_yes, int if_no) {
    GRect quick_view_bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    GRect full_bounds = layer_get_bounds(window_get_root_layer(main_window));

    if (quick_view_bounds.size.h == full_bounds.size.h) {
        return if_no;
    } else {
        return if_yes;
    }
}

/// @brief Updates time from system and sets time degree variables
void update_time() {
    time_t temp = time(NULL);
    struct tm *t = localtime(&temp);

    strftime(date_char, sizeof(date_char), "%d", t);

    // saves hour as int
    actual_hour = t->tm_hour;

    // converts time to degrees
    min = 360 * t->tm_min / 60;
    hour = 360 * (t->tm_hour % 12 * 6 + t->tm_min / 10) / (12 * 6);
    sec = 360 * t->tm_sec / 60;
}

// Drawing functions ================================================

/// @brief Draws a hand from the center point
/// @param length Length of hand
/// @param width Width of hand
/// @param rot Rotation (in degrees)
/// @param color Color of hand
/// @param ctx Graphics context to draw with
static void draw_hand(int length, int width, int rot, GColor color, GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    GPoint center = grect_center_point(&bounds);

    GPoint end_point = {
        .x = center.x + (sin_lookup(DEG_TO_TRIGANGLE(rot)) * length / TRIG_MAX_RATIO),
        .y = center.y - (cos_lookup(DEG_TO_TRIGANGLE(rot)) * length / TRIG_MAX_RATIO)};

    graphics_context_set_stroke_color(ctx, color);
    graphics_context_set_stroke_width(ctx, width);
    graphics_draw_line(ctx, center, end_point);
}

/// @brief Draws a segmented hand (used for pride hand drawing)
/// @param length Length of hand
/// @param width Width of hand
/// @param rot Rotation (degrees)
/// @param segments Number of segments
/// @param colors Array of colors (HEX)
/// @param ctx Graphics context to draw with
static void draw_gay_hand(int length, int width, int rot, int segments, int colors[], GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    GPoint center = grect_center_point(&bounds);

    for (int i = 0; i < segments; i++) {
        GPoint point1 = {
            .x = center.x + (sin_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i) / TRIG_MAX_RATIO),
            .y = center.y - (cos_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i) / TRIG_MAX_RATIO),
        };

        GPoint point2 = {
            .x = center.x + (sin_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i + 1) / TRIG_MAX_RATIO),
            .y = center.y - (cos_lookup(DEG_TO_TRIGANGLE(rot)) * length / segments * (i + 1) / TRIG_MAX_RATIO)};

        graphics_context_set_stroke_color(ctx, GColorFromHEX(colors[i]));
        graphics_context_set_stroke_width(ctx, width);
        graphics_draw_line(ctx, point1, point2);
    }
}

/// @brief Draws the dotted hour tick background
/// @param ctx Graphics context to draw with
static void draw_dot_bg(GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    GPoint center = grect_center_point(&bounds);

    graphics_context_set_fill_color(ctx, settings.hour_tick_color);

    int dist_from_center;
    dist_from_center = if_quickview_else(50, 68);

    for (int i = 0; i < settings.num_of_dots; i++) {
        int angle = TRIG_MAX_ANGLE * i / settings.num_of_dots;

        GPoint dot = {
            .x = center.x + sin_lookup(angle) * dist_from_center / TRIG_MAX_RATIO,
            .y = center.y - cos_lookup(angle) * dist_from_center / TRIG_MAX_RATIO};

        graphics_fill_circle(ctx, dot, settings.hour_tick_size);
    }
}

/// @brief Draws a background with lines pointing to the center
/// @param ctx Graphics context to draw with
/// @param dist_from_center Distance from center to stop lines (radius-wise)
static void draw_line_bg(GContext *ctx, int dist_from_center) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    GPoint center = grect_center_point(&bounds);

    graphics_context_set_stroke_color(ctx, settings.hour_tick_color);
    graphics_context_set_stroke_width(ctx, settings.hour_tick_size * 2);

    int second_dist = 100;

    for (int i = 0; i < settings.num_of_dots; i++) {
        int angle = TRIG_MAX_ANGLE * i / settings.num_of_dots;

        GPoint point1 = {
            .x = center.x + sin_lookup(angle) * dist_from_center / TRIG_MAX_RATIO,
            .y = center.y - cos_lookup(angle) * dist_from_center / TRIG_MAX_RATIO};

        GPoint point2 = {
            .x = center.x + sin_lookup(angle) * second_dist / TRIG_MAX_RATIO,
            .y = center.y - cos_lookup(angle) * second_dist / TRIG_MAX_RATIO};

        graphics_draw_line(ctx, point1, point2);
    }
}

// Update procedures ================================================

/// @brief Update procedure for drawing hour and minute hands and center dot
/// @param layer Layer to draw on
/// @param ctx Graphics context to draw with
void hands_draw_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    
    // makes min color the color of the background when the pride hand is enabled
    GColor min_hand_color = settings.flag == 0 ? settings.min_color : settings.bg_color;

    // length of hands depending on quickview
    int m_length = if_quickview_else(40, 55);
    int h_length = if_quickview_else(28, 38);

    // drawing hands
    draw_hand(m_length, settings.hand_width, min, min_hand_color, ctx);
    draw_hand(h_length, settings.hand_width, hour, settings.hour_color, ctx);

    // drawing the dot
    graphics_context_set_fill_color(ctx, settings.dot_color);
    graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 4);
}

/// @brief Update procedure for drawing the seconds hand
/// @param layer Layer to draw on
/// @param ctx Graphics context to draw with
void draw_sec_update_proc(Layer *layer, GContext *ctx) {
    int s_length = if_quickview_else(40, 55);

    draw_hand(s_length, settings.second_width, sec, settings.sec_color, ctx);
}

/// @brief Update procedure for drawing the hour marks
/// @param layer Layer to draw on
/// @param ctx Graphics context to draw with
void draw_hour_marks_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    GPoint center = grect_center_point(&bounds);

    // switch that controls which hour mark type to draw
    switch (settings.hour_tick_type) {
        // dot bg
        case DOT:
            draw_dot_bg(ctx);
            break;

        // short line bg (draws rect over line bg)
        case LINES:
            draw_line_bg(ctx, 20);
            
            // scale of bg to reduce by when drawing rectangle over lines
            double x_scale = .9;
            double y_scale = .9;
            
            // draws rectangle w/ color of background over lines to make them short
            graphics_context_set_fill_color(ctx, settings.bg_color);
            graphics_fill_rect(
                ctx,
                GRect(
                    center.x - center.x * x_scale,
                    center.y - center.y * y_scale,
                    bounds.size.w * x_scale,
                    bounds.size.h * y_scale),
                0,
                GCornerNone);

            break;

        // longer line bg (lines not cut off)
        case LONG_LINES:
            draw_line_bg(
                ctx,
                PBL_IF_ROUND_ELSE(
                    80,
                    if_quickview_else(54, 70)));
            break;

        // no hour markers
        case NONE:
            break;
    }
}

/// @brief Update procedure for drawing the pride minute hand
/// @param layer Layer to draw on
/// @param ctx Graphics context to draw with
void draw_gay_hand_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));

    // draws the hand itself
    int hand_length = if_quickview_else(40, 55);
    draw_gay_hand(
        hand_length, 
        settings.hand_width, 
        min,
        num_stripes[settings.flag],
        flag_colors[settings.flag],
        ctx);

    // draws another dot on top
    graphics_context_set_fill_color(ctx, settings.dot_color);
    graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 4);
}

/// @brief Update procedure for drawing the "pebble" logo text
/// @param layer Layer to draw on
/// @param ctx Graphics context to draw with
void pebble_text_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));

    int pebb_y_offset = if_quickview_else(25, 40);

    graphics_context_set_text_color(ctx, settings.pebble_color);
    graphics_draw_text(
        ctx, 
        "pebble", 
        fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
        GRect(0, pebb_y_offset, bounds.size.w, 50),
        GTextOverflowModeTrailingEllipsis, 
        GTextAlignmentCenter,
        0);
}

/// @brief Update procedure for drawing the date text
/// @param layer Layer to draw on
/// @param ctx Graphics context to draw with
void date_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));

    int square_quickview_offset = if_quickview_else(70, 55);

    graphics_context_set_text_color(ctx, settings.date_color);
    graphics_draw_text(
        ctx,
        date_char,
        fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
        GRect(
            bounds.size.w - PBL_IF_ROUND_ELSE(70, square_quickview_offset),
            bounds.size.h / 2 - 12, 
            50, 
            20), 
        GTextOverflowModeTrailingEllipsis,
        GTextAlignmentCenter,
        0);
}