#pragma once

typedef struct ClaySettings {
    int flag_number;
    int hand_width;
    int second_width;
    bool enable_seconds;
    GColor hour_color;
    GColor min_color;
    GColor dot_color;
    GColor sec_color;
    GColor hour_tick_color;
} ClaySettings;