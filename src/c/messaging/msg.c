#include <pebble.h>
#include "msg.h"
#include "../main.h"
#include "../config/cfg.h"

// creates tuples with values of message keys from settings and assigns to setting vars
static void inbox_recieved_handler(DictionaryIterator *iter, void *ctx) {
    Tuple *hWidth_t = dict_find(iter, MESSAGE_KEY_HandWidthKey);
    if (hWidth_t) {
        settings.hand_width = hWidth_t->value->int32;
    }

    Tuple *sWidth_t = dict_find(iter, MESSAGE_KEY_SecondWidthKey);
    if (sWidth_t) {
        settings.second_width = sWidth_t->value->int32;
    }

    Tuple *dotNum_t = dict_find(iter, MESSAGE_KEY_DotNumKey);
    if (dotNum_t) {
        settings.num_of_dots = atoi(dotNum_t->value->cstring);
    }

    Tuple *h_color_t = dict_find(iter, MESSAGE_KEY_HourColorKey);
    if (h_color_t) {
        settings.hour_color = GColorFromHEX(h_color_t->value->int32);
    }

    Tuple *m_color_t = dict_find(iter, MESSAGE_KEY_MinColorKey);
    if (m_color_t) {
        settings.min_color = GColorFromHEX(m_color_t->value->int32);
    }

    Tuple *dot_color_t = dict_find(iter, MESSAGE_KEY_DotColorKey);
    if (dot_color_t) {
        settings.dot_color = GColorFromHEX(dot_color_t->value->int32);
    }

    Tuple *s_color_t = dict_find(iter, MESSAGE_KEY_SecColorKey);
    if (s_color_t) {
        settings.sec_color = GColorFromHEX(s_color_t->value->int32);
    }

    Tuple *tick_color_t = dict_find(iter, MESSAGE_KEY_TickColorKey);
    if (tick_color_t) {
        settings.hour_tick_color = GColorFromHEX(tick_color_t->value->int32);
    }

    Tuple *dot_size_t = dict_find(iter, MESSAGE_KEY_HourTickSizeKey);
    if (dot_size_t) {
        settings.hour_tick_size = dot_size_t->value->int32;
    }

    Tuple *dot_type_t = dict_find(iter, MESSAGE_KEY_DotTypeKey);
    if (dot_type_t) {
        settings.hour_tick_type = atoi(dot_type_t->value->cstring);
    }

    Tuple *flag_t = dict_find(iter, MESSAGE_KEY_FlagKey);
    if (flag_t) {
        settings.flag = atoi(flag_t->value->cstring);
    }

    Tuple *pebble_t = dict_find(iter, MESSAGE_KEY_PebbleBoolKey);
    if (pebble_t) {
        settings.enable_pebble_logo = pebble_t->value->int32 == 1;
    }

    Tuple *pebble_color_t = dict_find(iter, MESSAGE_KEY_PebbleColorKey);
    if (pebble_color_t) {
        settings.pebble_color = GColorFromHEX(pebble_color_t->value->int32);
    }

    Tuple *date_enable_t = dict_find(iter, MESSAGE_KEY_DateBoolKey);
    if (date_enable_t) {
        settings.enable_date = date_enable_t->value->int32 == 1;
    }

    Tuple *date_color_t = dict_find(iter, MESSAGE_KEY_DateColorKey);
    if (date_color_t) {
        settings.date_color = GColorFromHEX(date_color_t->value->int32);
    }

    Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BgColorKey);
    if (bg_color_t) {
        settings.bg_color = GColorFromHEX(bg_color_t->value->int32);
    }

    Tuple *bt_buzz_t = dict_find(iter, MESSAGE_KEY_BtBuzzKey);
    if (bt_buzz_t) {
        settings.enable_bt_buzz = bt_buzz_t->value->int32 == 1;
    }

    Tuple *enable_sec_hand_t = dict_find(iter, MESSAGE_KEY_EnableSecondHandKey);
    if (enable_sec_hand_t) {
        settings.enable_seconds_hand = enable_sec_hand_t->value->int32 == 1;
    }

    Tuple *enable_pride_bg_t = dict_find(iter, MESSAGE_KEY_EnablePrideBgKey);
    if(enable_pride_bg_t) {
        settings.enable_pride_bg = enable_pride_bg_t->value->int32 == 1;
    }

    Tuple *enable_pride_hand_t = dict_find(iter, MESSAGE_KEY_EnablePrideHandKey);
    if(enable_pride_hand_t) {
        settings.enable_pride_hand = enable_pride_hand_t->value->int32 == 1;
    }

    save_settings();
    update_stuff();
}

void init_msg() {
    app_message_register_inbox_received(inbox_recieved_handler);
    
    // calculates buffer size depending on the amount of tuples up there ^
    uint32_t buffer_size = dict_calc_buffer_size(20);

    app_message_open(buffer_size, buffer_size);
}