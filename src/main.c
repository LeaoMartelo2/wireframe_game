#include "../raylib/raylib.h"
#include "../raylib/rcamera.h"

int main(void) {

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    ToggleFullscreen();

    InitAudioDevice();

    while (!WindowShouldClose()) {
    }

    CloseWindow();

    return 0;
}
