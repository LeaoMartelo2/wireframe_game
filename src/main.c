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

    player_set_collision_map(&player, &map_geometry);

    while (!WindowShouldClose()) {

        /*player_update(&player);*/

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(player.camera);

        player_update(&player);

        draw_reference_point();

        draw_world(&map_geometry);

        player_draw3D(&player);

        EndMode3D();

        player_draw_hud(&player);

        DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1.0f, WHITE);

        EndDrawing();
    }

    geometry_array_free(&map_geometry);
    CloseWindow();

    return 0;
}
