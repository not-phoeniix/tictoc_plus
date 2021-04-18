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
    settings.hour_tick_color = GColorBlack;
    settings.enable_seconds = false;
};

void load_settings() {
    default_settings();
    //persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}