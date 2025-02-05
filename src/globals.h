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
} settings_t;

extern settings_t g_settings;

#endif // !_GLOBALS_H_
