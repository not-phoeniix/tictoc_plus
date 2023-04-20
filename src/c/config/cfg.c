#include <pebble.h>
#include "cfg.h"
#include "../main.h"

// sets these settings by default before loading from persistent storage
static void set_defaults() {
    // integers
    settings.hand_width = 4;
    settings.second_width = 2;
    settings.hour_tick_size = 3;
    settings.num_of_dots = 12;
    settings.flag = 0;
    // enums
    settings.hour_tick_type = DOT;
    // booleans
    settings.enable_seconds_hand = true;
    settings.enable_date = true;
    settings.enable_pebble_logo = true;
    settings.enable_bt_buzz = true;
    // colors
    settings.bg_color = GColorBlack;
    settings.hour_color = GColorWhite;
    settings.min_color = GColorWhite;
    settings.sec_color = GColorRed;
    settings.dot_color = GColorRed;
    settings.hour_tick_color = GColorWhite;
    settings.date_color = GColorRed;
    settings.pebble_color = GColorWhite;
}

// loading settings from persistent storage
void load_settings() {
    set_defaults();
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// save settings to persistent storage
void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}