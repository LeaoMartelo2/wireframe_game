#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "../raylib/raylib.h"
#include <stdio.h>

typedef enum : size_t {
    SCENE_MAINMENU = 0,
    SCENE_LEVEL_TEST1,
    SCENE_LEVEL_TEST2,
    SCENE_LEVEL_TEST3,
} LEVELS;

extern int close_application;

typedef struct {
    bool fullscreen;
    bool camera_tilt;
    bool play_music;

    float draw_distance;
    float coll_distance;
} settings_t;

extern settings_t g_settings;

typedef struct {
    bool draw_triggers;
} debug_t;

extern debug_t g_debug;

typedef struct {
    Model shotgun;
    Model axe;
    Model cabela;
} gmodels_t;

extern gmodels_t g_assets;

typedef struct {

    Sound item_pickup_sound[3];

    Sound door_open;

    Sound item_axe_equip[3];

    Sound item_shotgun_reload;

} gsounds_t;

extern gsounds_t g_sounds;

void load_global_assets();

#endif // !_GLOBALS_H_
