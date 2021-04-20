#include <pebble.h>
#include "cfg.h"
#include "../main.h"

extern ClaySettings settings;

static void default_settings() {
    settings.hand_width = 4;
    settings.second_width = 2;
    settings.hour_color = GColorWhite;
    settings.min_color = GColorWhite;
    settings.dot_color = GColorRed;
    settings.sec_color = GColorRed;
    settings.hour_tick_color = GColorDarkGray;
    settings.enable_seconds = true;
    settings.bg_color = GColorBlack;
    settings.enable_bg = true;
    settings.num_of_dots = 12;
    settings.hour_tick_size = 3;
    settings.dot_type = 2;
    settings.flag = 0;
};

void load_settings() {
    default_settings();
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}