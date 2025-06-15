#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "../raylib/raylib.h"
#include <stdio.h>

typedef struct {

    float life_time;

    bool finished;

} WF_timer;

void timer_start(WF_timer *timer, float lifetime);
void timer_update(WF_timer *timer);
bool timer_finished(WF_timer *timer);

typedef enum : size_t {
    SCENE_MAINMENU = 0,
    SCENE_LEVEL_TEST1,
    SCENE_LEVEL_TEST2,
    SCENE_LEVEL_TEST3,
} LEVELS;

extern int close_application;

typedef struct {
    bool is_paused;
} state_t;

extern state_t g_gamestate;

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

    Model key;
} gmodels_t;

extern gmodels_t g_assets;

/* this shit is stupid, but uts the best i can manage */
typedef struct {

    Sound generic_click;

    Sound item_pickup_sound[3];

    Sound door_open;

    Sound item_axe_equip[3];

    Sound item_shotgun_reload;

    Sound item_cabela_interact;

} gsounds_t;

extern gsounds_t g_sounds;

/* its fine to leak memory here, this will be loaded throuout the whole application running period*/
void load_global_assets();

#endif // !_GLOBALS_H_
