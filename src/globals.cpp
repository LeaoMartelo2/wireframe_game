#include "globals.h"

void timer_start(WF_timer *timer, float lifetime) {
    if (timer != NULL) {
        timer->life_time = lifetime;
    }
}

void timer_update(WF_timer *timer) {

    if (timer != NULL && timer->life_time > 0) {

        timer->life_time -= GetFrameTime();
    }

    if (timer->life_time <= 0) {
        timer->finished = true;
    }
}

bool timer_finished(WF_timer *timer) {

    if (timer != NULL) {
        return timer->life_time <= 0;
    }

    return false;
}

int close_application = 0;

state_t g_gamestate = {
    .is_paused = false,
};

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

gmodels_t g_assets = {0};

gsounds_t g_sounds = {0};

/* this is fucking stupid, but its still better than loading and unloading them each time */
void load_global_assets() {

    g_assets.shotgun = LoadModel("assets/models/low_poly_shotgun/shotgun.gltf");
    g_assets.axe = LoadModel("assets/models/low_poly_axe/axe.obj");
    g_assets.cabela = LoadModel("assets/models/cabela/cabela.obj");

    g_assets.key = LoadModel("assets/models/key/keyOGA.obj");

    g_sounds.generic_click = LoadSound("assets/sounds/snd_button.wav");

    g_sounds.item_pickup_sound[0] = LoadSound("assets/sounds/items/pickup.wav");
    g_sounds.item_pickup_sound[1] = LoadSound("assets/sounds/items/pickup1.wav");
    g_sounds.item_pickup_sound[2] = LoadSound("assets/sounds/items/pickup2.wav");

    g_sounds.door_open = LoadSound("assets/sounds/map_sounds/door_open.wav");

    g_sounds.item_axe_equip[0] = LoadSound("assets/sounds/items/axe/equip.wav");
    g_sounds.item_axe_equip[1] = LoadSound("assets/sounds/items/axe/equip1.wav");
    g_sounds.item_axe_equip[2] = LoadSound("assets/sounds/items/axe/equip2.wav");

    g_sounds.item_shotgun_reload = LoadSound("assets/sounds/items/shotgun/reload.wav");

    g_sounds.item_cabela_interact = LoadSound("assets/sounds/items/cabela/cabela.ogg");
}
