#include <pebble.h>
#include "main.h"
#include "drawing/drawing.h"
#include "config/cfg.h"
#include "messaging/msg.h"

// handlers/services ================================================

// handles buzzing w/ bluetooth status disconnect
static void bluetooth_callback(bool connected) {
    if (settings.enable_bt_buzz == true && !connected) {
        vibes_short_pulse();
    }
}

// handles changing/updating of time
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
    layer_mark_dirty(hands_layer);
    layer_mark_dirty(sec_hand_layer);
}

// subscribes to the timer service
static void timer_service_change(bool use_seconds) {
    tick_timer_service_unsubscribe();

    if (!use_seconds) {
        tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    } else {
        tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    }
}

// universal update function ========================================

/// @brief Redraws all layers and toggles visiblity depending on config
void update_stuff() {
    update_time();

    window_set_background_color(main_window, settings.bg_color);

    layer_mark_dirty(hands_layer);
    layer_mark_dirty(hour_tick_layer);
    layer_mark_dirty(sec_hand_layer);
    layer_mark_dirty(gay_layer);
    layer_mark_dirty(pebb_layer);
    layer_mark_dirty(date_layer);

    layer_set_hidden(sec_hand_layer, !settings.enable_seconds_hand);
    timer_service_change(settings.enable_seconds_hand);

    layer_set_hidden(gay_layer, settings.flag == 0);
    layer_set_hidden(pebb_layer, !settings.enable_pebble_logo);
    layer_set_hidden(date_layer, !settings.enable_date);
}

// window load and unload ===========================================

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    window_set_background_color(main_window, settings.bg_color);

    update_time();

    hour_tick_layer = layer_create(bounds);
    layer_set_update_proc(hour_tick_layer, draw_hour_marks_update_proc);
    layer_add_child(window_layer, hour_tick_layer);

    pebb_layer = layer_create(bounds);
    layer_set_update_proc(pebb_layer, pebble_text_update_proc);
    layer_add_child(window_layer, pebb_layer);

    date_layer = layer_create(bounds);
    layer_set_update_proc(date_layer, date_update_proc);
    layer_add_child(window_layer, date_layer);

    hands_layer = layer_create(bounds);
    layer_set_update_proc(hands_layer, hands_draw_update_proc);
    layer_add_child(window_layer, hands_layer);

    gay_layer = layer_create(bounds);
    layer_set_update_proc(gay_layer, draw_gay_hand_update_proc);
    layer_add_child(window_layer, gay_layer);

    sec_hand_layer = layer_create(bounds);
    layer_set_update_proc(sec_hand_layer, draw_sec_update_proc);
    layer_add_child(window_layer, sec_hand_layer);
    
    update_stuff();
}

static void main_window_unload() {
    layer_destroy(hour_tick_layer);
    layer_destroy(pebb_layer);
    layer_destroy(date_layer);
    layer_destroy(date_layer);
    layer_destroy(gay_layer);
    layer_destroy(sec_hand_layer);
}

// init and deinit ==================================================

static void init() {
    main_window = window_create();

    // subscribing to watch services
    timer_service_change(false);
    connection_service_subscribe((ConnectionHandlers){
        .pebble_app_connection_handler = bluetooth_callback
    });

    // setting window load & unload functions
    window_set_window_handlers(main_window, (WindowHandlers){
        .load = main_window_load,
        .unload = main_window_unload
    });

    init_msg();
    load_settings();

    window_stack_push(main_window, true);

    bluetooth_callback(connection_service_peek_pebble_app_connection());
}

static void deinit() {
    tick_timer_service_unsubscribe();
    connection_service_unsubscribe();
    window_destroy(main_window);
}

// main =============================================================

int main(void) {
    init();
    app_event_loop();
    deinit();
}
