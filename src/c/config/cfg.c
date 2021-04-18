#include <pebble.h>
#include "cfg.h"
#include "../main.h"

extern ClaySettings settings;

static void default_settings() {
    settings.hand_width = 8;
    settings.hour_color = GColorRed;
    settings.min_color = GColorWhite;
    settings.dot_color = GColorWhite;
    settings.hour_tick_color = GColorBlack;
};

void load_settings() {
    default_settings();
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}