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

gmodels_t g_assets = {};

void load_global_assets() {
    g_assets.shotgun = LoadModel("assets/models/low_poly_shotgun/shotgun.gltf");
    g_assets.axe = LoadModel("assets/models/low_poly_axe/axe.obj");
    g_assets.cabela = LoadModel("assets/models/cabela/cabela.obj");
}
