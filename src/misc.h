#ifndef MISC_H_
#define MISC_H_

#include "../raylib/raylib.h"
#include "include/lognest.h"

inline void raylib_setup(void) {

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    lognest_trace("[Raylib] Window opened.");
    SetTraceLogLevel(LOG_NONE);

    SetTargetFPS(60);
    ToggleFullscreen();
    lognest_trace("[Raylib] Enabled Fullscreen.");
}

#endif // !MISC_H_
