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
    settings.hour_tick_color = GColorWhite;
    settings.bg_color = GColorBlack;
    settings.enable_bg = true;
    settings.num_of_dots = 12;
    settings.hour_tick_size = 3;
    settings.dot_type = 2;
    settings.flag = 0;
    settings.pebble_color = GColorWhite;
    settings.enable_pebble = true;
    settings.date_color = GColorRed;
    settings.enable_date = true;
    settings.do_bt_buzz = true;
    settings.sec_start = 0;
    settings.sec_end = 24;
};

void load_settings() {
    default_settings();
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}