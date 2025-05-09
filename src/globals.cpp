#include "globals.h"

int close_application = 0;

settings_t g_settings = {
    .fullscreen = true,
    .camera_tilt = true,
    .play_music = true,

    .draw_distance = 10000.0f,
    .coll_distance = 10000.0f,
};

debug_t g_debug = {
    .draw_triggers = false,
};
