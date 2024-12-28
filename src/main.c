#include "../raylib/raylib.h"
#include "../raylib/rcamera.h"
#include "geometry.h"
#include "player.h"
#include <stdlib.h>

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

        /*DrawBoundingBox(player.collision.bounding_box, GREEN);*/

        EndMode3D();

        DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1.0f, WHITE);

        DrawText(TextFormat("Position:\nX: %.2f, Y: %.2f, Z: %.2f\n"
                            "Input:\n -> Forward: %f\n -> Sideways: %f\n -> Upwards: %f\n"
                            "Velocity:\n -> Forward: %.2f\n -> Sideways: %.2f\n",
                            player.pos.x, player.pos.y, player.pos.z,
                            player.input.forwards, player.input.sideways, player.input.up_down,
                            player.velocity.forwards, player.velocity.sideways),
                 10, 10,
                 20, WHITE);

        EndDrawing();
    }

    geometry_array_free(&map_geometry);
    CloseWindow();

    return 0;
}
