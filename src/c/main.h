#pragma once

typedef struct ClaySettings {
    int flag_number;
    int hand_width;
    int second_width;
    int hour_tick_size;
    bool enable_seconds;
    bool enable_bg;
    GColor hour_color;
    GColor min_color;
    GColor dot_color;
    GColor sec_color;
    GColor hour_tick_color;
    GColor bg_color;
    int num_of_dots;
    int dot_type;
} ClaySettings;

void update_stuff();