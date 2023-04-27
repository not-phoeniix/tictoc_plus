#pragma once

#include <pebble.h>

#define PBL_IS_ROUND PBL_IF_ROUND_ELSE(true, false)
#define PBL_IS_COLOR PBL_IF_COLOR_ELSE(true, false)

// different types of hour marks
enum hour_tick_type {
    DOT,
    LINES,
    LONG_LINES,
    NONE
} hour_tick_type;

typedef struct ClaySettings {
    int hand_width;
    int second_width;
    int hour_tick_size;
    int num_of_dots;
    int flag;
    int rot_flag;
    enum hour_tick_type hour_tick_type;
    bool enable_pride_bg;
    bool enable_pride_hand;
    bool enable_seconds_hand;
    bool enable_date;
    bool enable_pebble_logo;
    bool enable_bt_buzz;
    GColor bg_color;
    GColor hour_color;
    GColor min_color;
    GColor sec_color;
    GColor dot_color;
    GColor hour_tick_color;
    GColor date_color;
    GColor pebble_color;
} ClaySettings;

// creating all the layer/window object stuff for the program
Window *main_window;
Layer *pride_bg_layer;
Layer *hands_layer;
Layer *sec_hand_layer;
Layer *hour_tick_layer;
Layer *pride_hand_layer;
Layer *pebb_layer;
Layer *date_layer;
ClaySettings settings;

void update_stuff();