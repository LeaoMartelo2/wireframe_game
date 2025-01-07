#include "../raylib/raylib.h"
#include "../raylib/rcamera.h"
#include "geometry.h"
#include "player.h"

int main(void) {

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Wireframe Engine");
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    ToggleFullscreen();

    DisableCursor();

    InitAudioDevice();

    Player player;

    while (!WindowShouldClose()) {

        player.update();

        BeginDrawing();
        {

            ClearBackground(BLACK);

            BeginMode3D(player.camera);
            {
                draw_reference_point();
                player.draw();
                player.debug_3d();
            }
            EndMode3D();

            player.draw_hud();

            DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1.0f, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
