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

#endif // !_GLOBALS_H_
