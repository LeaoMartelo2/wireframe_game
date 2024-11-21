#include "player.h"
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {

    InitWindow(1366, 768, "Boomer Shooter");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();
    SetTargetFPS(60);

    DisableCursor();

    Player player = {0};
    player.camera = (Camera3D){0};
    player.camera.position = (Vector3){10, 10, 10};
    player.camera.target = Vector3Zero();
    player.camera.up = (Vector3){0, 1, 0};
    player.camera.fovy = 90.0f;
    player.camera.projection = CAMERA_PERSPECTIVE;
    player.postition = (Vector3){10, 10, 10};
    player.turn_A = false;
    player.turn_D = false;
    player.move_speed = 500.0f;
    player.cam_rot_speed = 0.1f;
    player.cam_rol_scale = 0.1f;
    player.gravity = 0;
    player.bounding_box_size = (Vector3){5, 10, 5};

    Ray ray = {0};

    Vector3 cube_pos = {0};

    float size = 10.0f;
    Mesh cube_mesh = GenMeshCube(size, size, size);
    Model cube_model = LoadModelFromMesh(cube_mesh);

    Mesh floor_mesh = GenMeshCube(600, 10, 600);
    Model floor_model = LoadModelFromMesh(floor_mesh);
    Vector3 floor_pos = {0, 0, 0};

    double rot = 0;

    Texture2D billboard = LoadTexture("billboard.png");
    Vector3 billboard_pos = cube_pos;

    billboard_pos.y += 20;

    while (!WindowShouldClose()) {

        move_cam(&player);
        update_player(&player);

        rot++;

        BeginDrawing();
        {
            ClearBackground(BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ray = GetScreenToWorldRay((Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0 - 2},
                                          player.camera);
            }

            BeginMode3D(player.camera);
            {

                DrawModelWires(floor_model, floor_pos, 1, RED);

                DrawModelWiresEx(cube_model, cube_pos, Vector3One(), rot, Vector3One(), BLUE);

                DrawBillboard(player.camera, billboard, billboard_pos, 10, WHITE);

                /*DrawCubeWiresV((Vector3){player.postition.x, player.postition.y - 5, player.postition.z},*/
                /*player.bounding_box_size, ORANGE);*/

                DrawRay(ray, YELLOW);

                if (check_colision_test(&player, floor_model, floor_pos)) {
                    printf("CollisonA\n");
                }
            }
            EndMode3D();

            DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1, GREEN);
            DrawText(TextFormat("x:%f\ny:%f\nz:%f", player.postition.x, player.postition.y, player.postition.z),
                     5, 5, 30, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
