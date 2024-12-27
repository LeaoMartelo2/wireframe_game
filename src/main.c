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

    DisableCursor();

    InitAudioDevice();

    Player player = {0};
    player_setup(&player);

    while (!WindowShouldClose()) {
        update_player(&player);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(player.camera);

        DrawCube(Vector3Zero(), 50, 50, 50, RED);

        DrawBoundingBox(player.collision.bounding_box, GREEN);

        EndMode3D();

        DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1.0f, WHITE);

        DrawText(TextFormat("pos:\nX:%.2f, Y:%.2f, Z:%.2f\n"
                            "Camera target:\nX: %.2f, Y:%.2f, Z:%.2f\n"
                            "Input:\nFowards/backwards: %f, sideways: %f\n"
                            "Velocity:\nForwards/backwards: %.2f, %.2f\n",
                            player.pos.x, player.pos.y, player.pos.z,
                            player.camera.target.x, player.camera.target.y, player.camera.target.z,
                            player.input.forwards, player.input.sideways,
                            player.velocity.forwards, player.velocity.sideways),
                 10, 10,
                 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
