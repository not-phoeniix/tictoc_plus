#pragma once

typedef struct ClaySettings {
    int hand_width;
    int second_width;
    int hour_tick_size;
    bool enable_bg;
    bool enable_pebble;
    bool enable_date;
    bool do_bt_buzz;
    GColor hour_color;
    GColor min_color;
    GColor dot_color;
    GColor sec_color;
    GColor hour_tick_color;
    GColor bg_color;
    GColor pebble_color;
    GColor date_color;
    int num_of_dots;
    int dot_type;
    int flag;
    int sec_start;
    int sec_end;
} ClaySettings;

void update_stuff();