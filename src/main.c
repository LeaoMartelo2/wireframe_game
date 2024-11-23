#include "models.h"
#include "player.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdbool.h>

#define MAX_SOLIDS 10

int main(void) {

    InitWindow(1366, 768, "Boomer Shooter");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    /*SetWindowState(FLAG_MSAA_4X_HINT);*/
    MaximizeWindow();
    SetTargetFPS(60);

    DisableCursor();

    Player player = {0};

    init_player(&player);

    load_map(player.geometry, &player.geometry_count, player.ground_geometry);

    Ray ray = {0};

    Vector3 cube_pos = {0, 15, 0};
    float cube_size = 10.0f;
    Mesh cube_mesh = GenMeshCube(cube_size, cube_size, cube_size);
    Model cube_model = LoadModelFromMesh(cube_mesh);

    double rot = 0;

    Texture2D billboard = LoadTexture("sprites/billboard.png");
    Vector3 billboard_pos = cube_pos;

    billboard_pos.y += 20;

    Model shotgun = LoadModel("models/low_poly_shotgun/shotgun.gltf");

    Vector3 item_pos = {500, 15, 0};

    while (!WindowShouldClose()) {

        move_player(&player);
        update_player(&player);

        rot++;
        item_pos.y += sinf(GetTime()) * 0.1f;

        BeginDrawing();
        {
            ClearBackground(BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ray = GetScreenToWorldRay((Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0 - 2},
                                          player.camera);
            }

            BeginMode3D(player.camera);
            {

                draw_map(player.geometry, player.geometry_count, player.ground_geometry);

                DrawModelEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLACK);
                DrawModelWiresEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLUE);

                DrawModelEx(shotgun, item_pos, Vector3One(), rot, (Vector3){10, 10, 10}, GetColor(0x181818FF));
                DrawModelWiresEx(shotgun, item_pos, Vector3One(), rot, (Vector3){10, 10, 10}, WHITE);

                DrawBillboard(player.camera, billboard, billboard_pos, 10, WHITE);

                DrawRay(ray, YELLOW);

                draw_viewmodel(&player, shotgun);
            }
            EndMode3D();

            DrawFPS(1, 1);

            DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1, GREEN);
            DrawText(TextFormat("x:%f\ny:%f\nz:%f", player.postition.x, player.postition.y, player.postition.z),
                     5, 30, 30, WHITE);

            DrawText(TextFormat("velo forward:%f\nvelo sideways:%f\nvelo vertical:%f",
                                player.velocity.x, player.velocity.z, player.velocity.y),
                     5, 150, 30, WHITE);

            /* DrawText(TextFormat("viewmodel:\nx:%f\ny:%f\nz:%f",
                                player.viewmodel_pos.x, player.viewmodel_pos.y, player.viewmodel_pos.z),
                     5, 300, 30, WHITE); */
        }
        EndDrawing();
    }

    for (int i = 0; i < player.geometry_count; i++) {
        UnloadModel(player.geometry[i].model);
    }

    CloseWindow();

    return 0;
}
