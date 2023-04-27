#include <pebble.h>
#include "pride.h"
#include "../main.h"

// definitions of each flag's hex code color variables
static int clear[] = {0x000000};
static int pride[] = {0xFF0000, 0xFFAA00, 0xFFFF00, 0x00FF00, 0x0000FF, 0xAA00AA};
static int trans[] = {0x00AAFF, 0xFF55FF, 0xFFFFFF, 0xFF55FF, 0x00AAFF};
static int bisexual[] = {0xAA0055, 0xAA0055, 0xAA55AA, 0x0000FF, 0x0000FF};
static int lesbian[] = {0xFF0000, 0xFF5500, 0xFFAA00, 0xFFFFFF, 0xAA55AA, 0xAA00AA, 0xAA0055};
static int pansexual[] = {0xFF00FF, 0xFFFF00, 0x00AAFF};
static int asexual[] = {0x000000, 0xAAAAAA, 0xFFFFFF, 0xAA00AA};
static int nonbinary[] = {0xFFFF55, 0xFFFFFF, 0xAA55AA, 0x000000};

// array that holds the above arrays
static int *flag_colors[] = {clear, pride, trans, bisexual, lesbian, pansexual, asexual, nonbinary};
static int num_stripes[] = {0, 6, 5, 5, 7, 3, 4, 4};

static void draw_flag(int segments, int colors[], GContext *ctx) {
    // setting bounds for main window's layer to draw flag for ENTIRE SCREEN
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));

    // height and width of each bar in pixels
    int bar_height, bar_width;

    // switch for the rotation value of the flag
        // default = 0 degrees
        // 1 = 90 degrees
        // 2 = 180 degrees
        // 3 = 270 degrees
    switch (settings.rot_flag) {
        default:
            // everything is normal in the height for each bar
            bar_height = bounds.size.h / segments + (bounds.size.h % segments != 0);
            bar_width = bounds.size.w;
            break;
        case 1:
            // 90 degrees, the height is set to the full screen height 
                // and the width is the one being segmented
            bar_height = bounds.size.h;
            bar_width = -1 * bounds.size.w / segments - (bounds.size.w % segments != 0);
            break;
        case 2:
            // 180 degrees, basically just default inverted
            bar_height = -1 * bounds.size.h / segments - (-1 * bounds.size.h % segments != 0);
            bar_width = bounds.size.w;
            break;
        case 3:
            // 270 degrees, basically just 90 degrees inverted
            bar_height = bounds.size.h;
            bar_width = bounds.size.w / segments + (bounds.size.w % segments != 0);
    }

    // loop that draws a flag segment and repeats until all of 'em are drawn
    for (int i = 0; i < segments; i++) {
        // defines flag stripe rectangle without initialization 
        GRect flag_stripe;

        // switch statement for rotations again,
            // which sets the value of the GRect "flag_stripe" to the correct orientation 
            // and location values according to the loop number
        switch (settings.rot_flag) {
            default:
                // 0 degrees
                flag_stripe = GRect(0, bar_height * i, bar_width, bar_height);
                break;
            case 1:
                // 90 degrees
                flag_stripe = GRect(bounds.size.w + (bar_width * i), 0, bar_width, bar_height);
                break;
            case 2:
                // 180 degrees
                flag_stripe = GRect(0, bounds.size.h + (bar_height * i), bar_width, bar_height);
                break;
            case 3:
                // 270 degrees
                flag_stripe = GRect(bar_width * i, 0, bar_width, bar_height);
                break;
        }

        // sets the fill color of the stripe to that of the current segment
            // then draws the actual rectangle
        graphics_context_set_fill_color(ctx, GColorFromHEX(colors[i]));
        graphics_fill_rect(ctx, flag_stripe, 0, GCornerNone);
    }
}

// update_proc, sets function to call when marking layer as dirty
void pride_update_proc(Layer *layer, GContext *ctx) {
    // draws pride flag
    draw_flag(num_stripes[settings.flag], flag_colors[settings.flag], ctx);

    // data for screen sizing and border
    GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
    int border_size = PBL_IS_ROUND ? 10 : 7;

    graphics_context_set_fill_color(ctx, settings.bg_color);

    // draws layer on top of flag to make it look like an outline
    if(PBL_IS_ROUND) {
        // draws circle for round pebbles
        graphics_fill_circle(
            ctx,
            GPoint(bounds.size.w / 2, bounds.size.h / 2),
            bounds.size.w / 2 - border_size);
    } else {
        // draws rect for rectangular pebbles
        graphics_fill_rect(
            ctx,
            GRect(
                bounds.origin.x + border_size, 
                bounds.origin.y + border_size,
                bounds.size.w - (2 * border_size),
                bounds.size.h - (2 * border_size)),
            0,
            GCornerNone);
    }
}
