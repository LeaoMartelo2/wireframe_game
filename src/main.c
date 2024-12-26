#include "../raylib/raylib.h"
#include "../raylib/rcamera.h"
#include "player.h"

int main(void) {

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    ToggleFullscreen();

    InitAudioDevice();

    Player player = {0};
    player_setup(&player);

    while (!WindowShouldClose()) {

        BeginDrawing();
        BeginMode3D(player.camera);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
