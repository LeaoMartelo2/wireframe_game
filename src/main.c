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

    Player player = {0};
    player_setup(&player);

    Geometry_Array map_geometry;

    geometry_array_init(&map_geometry);

    test_cube(&map_geometry);

    while (!WindowShouldClose()) {
        update_player(&player);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(player.camera);

        draw_reference_point();

        draw_world(&map_geometry);

        DrawBoundingBox(player.collision.bounding_box, GREEN);

        EndMode3D();

        DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1.0f, WHITE);

        player_debug(&player);

        EndDrawing();
    }

    geometry_array_free(&map_geometry);
    CloseWindow();

    return 0;
}
