#ifndef MISC_H_
#define MISC_H_

#include "../raylib/raylib.h"
#include "include/lognest.h"

inline void raylib_setup() {

    /*SetWindowState(FLAG_MSAA_4X_HINT);*/
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    SetTraceLogLevel(LOG_NONE);

    SetTargetFPS(60);
    lognest_debug("[Raylib] Set target FPS to 60");

    ToggleFullscreen();
    lognest_trace("[Raylib] Enabled Fullscreen");
}

#endif // !MISC_H_
